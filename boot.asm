;******************************************************************************
;* BOOT  v4.6.4                                                               *
;* Copyright (c) 1996-2010 Texas Instruments Incorporated                     *
;******************************************************************************

;****************************************************************************
;* BOOT.ASM
;*
;* THIS IS THE INITAL BOOT ROUTINE FOR TMS470 C++ PROGRAMS.
;* IT MUST BE LINKED AND LOADED WITH ALL C++ PROGRAMS.
;*
;* THIS MODULE PERFORMS THE FOLLOWING ACTIONS:
;*   1) ALLOCATES THE STACK AND INITIALIZES THE STACK POINTER
;*   2) CALLS AUTO-INITIALIZATION ROUTINE
;*   3) CALLS THE FUNCTION MAIN TO START THE C++ PROGRAM
;*   4) CALLS THE STANDARD EXIT ROUTINE
;*
;* THIS MODULE DEFINES THE FOLLOWING GLOBAL SYMBOLS:
;*   1) __stack     STACK MEMORY AREA
;*   2) _c_int00    BOOT ROUTINE
;*
;****************************************************************************
   .if  __TI_TMS470_V7M3__
	.thumbfunc _c_int00
   .else
	.armfunc _c_int00
   .endif

;****************************************************************************
; Accomodate different lowerd names in different ABIs
;****************************************************************************
   .if   __TI_EABI_ASSEMBLER
        .asg	_args_main,   ARGS_MAIN_RTN
        .asg	exit,         EXIT_RTN
        .asg    main_func_sp, MAIN_FUNC_SP
   .elseif __TI_ARM9ABI_ASSEMBLER | .TMS470_32BIS
        .asg	__args_main,   ARGS_MAIN_RTN
        .asg	_exit,         EXIT_RTN
        .asg    _main_func_sp, MAIN_FUNC_SP
   .else
        .asg	$_args_main,   ARGS_MAIN_RTN
        .asg	$exit,         EXIT_RTN
        .asg    _main_func_sp, MAIN_FUNC_SP
   .endif

   .if .TMS470_16BIS

		;****************************************************************************
		;*  16 BIT STATE BOOT ROUTINE                                               *
		;****************************************************************************

	   .if __TI_TMS470_V7M3__
		.state16
	   .else
		.state32
	   .endif

		.global	__stack
		;***************************************************************
		;* DEFINE THE USER MODE STACK (DEFAULT SIZE IS 512)
		;***************************************************************
		__stack:.usect	".stack", 0, 4

		.global	_c_int00

		;***************************************************************
		;* FUNCTION DEF: _c_int00
		;***************************************************************
		_c_int00: .asmfunc

		.if !__TI_TMS470_V7M3__
			.if __TI_NEON_SUPPORT__ | __TI_VFP_SUPPORT__
		        ;*------------------------------------------------------
				;* SETUP PRIVILEGED AND USER MODE ACCESS TO COPROCESSORS
				;* 10 AND 11, REQUIRED TO ENABLE NEON/VFP
				;* COPROCESSOR ACCESS CONTROL REG
				;* BITS [23:22] - CP11, [21:20] - CP10
				;* SET TO 0b11 TO ENABLE USER AND PRIV MODE ACCESS
		        ;*------------------------------------------------------
				MRC      p15,#0x0,r0,c1,c0,#2
		        MOV      r3,#0xf00000
				ORR      r0,r0,r3
		        MCR      p15,#0x0,r0,c1,c0,#2

			    ;*------------------------------------------------------
				; SET THE EN BIT, FPEXC[30] TO ENABLE NEON AND VFP
			    ;*------------------------------------------------------
		      	MOV      r0,#0x40000000
		        FMXR     FPEXC,r0
			.endif

			;------------------------------------------------------
			;* SET TO SYSTEM MODE
		    ;*------------------------------------------------------
		    CPS		#0x1F
		    ;MRS     r0, cpsr
		    ;BIC     r0, r0, #0x1F  ; CLEAR MODES
		    ;ORR     r0, r0, #0x1F  ; SET SYSTEM MODE
		    ;MSR     cpsr_cf, r0

	        ;*------------------------------------------------------
			;* CHANGE TO 16 BIT STATE
	        ;*------------------------------------------------------
			ADD	r0, pc, #1
			BX	r0

			.state16
		.endif

		;*------------------------------------------------------
	    ;* INITIALIZE THE USER MODE STACK
	    ;*------------------------------------------------------
		LDR     r0, c_stack
		MOV		sp, r0
	    LDR     r0, c_STACK_SIZE
		ADD		sp, r0

		;*-----------------------------------------------------
		;* ALIGN THE STACK TO 64-BITS IF EABI.
		;*-----------------------------------------------------
		.if __TI_EABI_ASSEMBLER
			MOV	r7, sp
			MOV	r0, #0x07
			BIC     r7, r0         ; Clear upper 3 bits for 64-bit alignment.
			MOV	sp, r7
		.endif

		;*-----------------------------------------------------
		;* SAVE CURRENT STACK POINTER FOR SDP ANALYSIS
		;*-----------------------------------------------------
		LDR	r0, c_mf_sp
		MOV	r7, sp
		STR	r7, [r0]

        ;*------------------------------------------------------
        ;* Perform all the required initilizations:
        ;*   - Process BINIT Table
        ;*   - Perform C auto initialization
        ;*   - Call global constructors
        ;*------------------------------------------------------
        BL      __TI_auto_init

        ;*------------------------------------------------------
		;* CALL APPLICATION
        ;*------------------------------------------------------
        BL      ARGS_MAIN_RTN

        ;*------------------------------------------------------
		;* IF APPLICATION DIDN'T CALL EXIT, CALL EXIT(1)
        ;*------------------------------------------------------
        MOV     r0, #1
        BL      EXIT_RTN

        ;*------------------------------------------------------
		;* DONE, LOOP FOREVER
        ;*------------------------------------------------------
L1:     B	L1
	.endasmfunc

   .else           ; !.TMS470_16BIS

;****************************************************************************
;*  32 BIT STATE BOOT ROUTINE                                               *
;****************************************************************************

	.global	__stack
;***************************************************************
;* DEFINE THE USER MODE STACK (DEFAULT SIZE IS 512)
;***************************************************************
__stack:.usect	".stack", 0, 4

	.global	_c_int00
	.global irqStack
    .global kernelStack
    .global abortStack
    .global systemStack

;***************************************************************
;* FUNCTION DEF: _c_int00
;***************************************************************
_c_int00: .asmfunc

      ; SET IRQ-Stack
    CPS   0x12
    LDR   sp, c_r13_irq

    ; SET SWI-Stack / Kernel-Stack
    CPS   0x13
    LDR   sp, c_r13_kernel

	; SET ABT-Stack
     CPS 0x17
     LDR   sp, c_r13_irq

     ; SET System-Stack
    CPS   0x1F
    LDR   sp, c_r13_system

    ; Disable Interrupts
    MRS   R12, CPSR
    BIC   R12, R12, #192
    MSR   CPSR_cf, R12

	.if __TI_NEON_SUPPORT__ | __TI_VFP_SUPPORT__
        ;*------------------------------------------------------
		;* SETUP PRIVILEGED AND USER MODE ACCESS TO COPROCESSORS
		;* 10 AND 11, REQUIRED TO ENABLE NEON/VFP
		;* COPROCESSOR ACCESS CONTROL REG
		;* BITS [23:22] - CP11, [21:20] - CP10
		;* SET TO 0b11 TO ENABLE USER AND PRIV MODE ACCESS
        ;*------------------------------------------------------
		MRC      p15,#0x0,r0,c1,c0,#2
        MOV      r3,#0xf00000
		ORR      r0,r0,r3
        MCR      p15,#0x0,r0,c1,c0,#2

        ;*------------------------------------------------------
		; SET THE EN BIT, FPEXC[30] TO ENABLE NEON AND VFP
        ;*------------------------------------------------------
      	MOV      r0,#0x40000000
        FMXR     FPEXC,r0
	.endif

    ;*------------------------------------------------------
	;* SET TO IRQ MODE with IRQ and FIQ disabled
    ;*------------------------------------------------------
    MRS     r0, cpsr
    BIC     r0, r0, #0x1F  ; CLEAR MODES
    ORR     r0, r0, #0x12  ; SET IRQ MODE
    MSR     cpsr_cf, r0

	;/* set sp for IRQ
	LDR     sp, c_r13_irq

    ;*------------------------------------------------------
	;* SET TO Super-User MODE with IRQ and FIQ disabled
    ;*------------------------------------------------------
    MRS     r0, cpsr
    BIC     r0, r0, #0x1F  ; CLEAR MODES
    ORR     r0, r0, #0x1F  ; SET Super-User MODE
    MSR     cpsr_cf, r0

    ;*------------------------------------------------------
    ;* INITIALIZE THE USER MODE STACK
    ;*------------------------------------------------------
	LDR     sp, c_stack
    LDR     r0, c_STACK_SIZE
	ADD	sp, sp, r0

	;*-----------------------------------------------------
	;* ALIGN THE STACK TO 64-BITS IF EABI.
	;*-----------------------------------------------------
	.if __TI_EABI_ASSEMBLER
		BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
	.endif

	;*-----------------------------------------------------
	;* SAVE CURRENT STACK POINTER FOR SDP ANALYSIS
	;*-----------------------------------------------------
	LDR	r0, c_mf_sp
	STR	sp, [r0]

    ;*------------------------------------------------------
    ;* Perform all the required initilizations:
    ;*   - Process BINIT Table
    ;*   - Perform C auto initialization
    ;*   - Call global constructors
    ;*------------------------------------------------------
    ;BL      __TI_auto_init

    ;*------------------------------------------------------
	;* CALL APPLICATION
    ;*------------------------------------------------------
    BL      ARGS_MAIN_RTN

    ;*------------------------------------------------------
	;* IF APPLICATION DIDN'T CALL EXIT, CALL EXIT(1)
    ;*------------------------------------------------------
    MOV     R0, #1
    BL      EXIT_RTN

    ;*------------------------------------------------------
	;* DONE, LOOP FOREVER
    ;*------------------------------------------------------
L1:     B	L1
	.endasmfunc

   .endif    ; !.TMS470_16BIS

;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************
c_stack			.long    __stack
c_STACK_SIZE  	.long    __STACK_SIZE
c_mf_sp	        .long    MAIN_FUNC_SP
;c_r13_irq		.long    0x8200C100
c_r13_irq       .long    irqStack
c_r13_kernel    .long    kernelStack
c_r13_abt       .long    abortStack
c_r13_system    .long    systemStack

	.if __TI_EABI_ASSEMBLER
        .data
        .align 4
_stkchk_called:
        .field          0,32
        .else
        .sect   ".cinit"
        .align  4
        .field          4,32
        .field          _stkchk_called+0,32
        .field          0,32

        .bss    _stkchk_called,4,4
        .symdepend ".cinit", ".bss"
        .symdepend ".cinit", ".text"
        .symdepend ".bss", ".text"
	.endif

;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************
	.global _stkchk_called
	.global	__STACK_SIZE
	.global ARGS_MAIN_RTN
	.global MAIN_FUNC_SP
	.global	EXIT_RTN
    .global __TI_auto_init

.end
