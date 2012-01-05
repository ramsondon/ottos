;***************************************************************
;* this is an arm-function
;***************************************************************

    .armfunc _c_int00
    .global  _c_int00

;***************************************************************
;* TI includes
;***************************************************************

    .asg    __args_main,   ARGS_MAIN_RTN
    .global ARGS_MAIN_RTN
    .global __TI_auto_init

;***************************************************************
;* Included addresses from the linker
;***************************************************************

    .global irqStack
    .global kernelStack
    .global abortStack
    .global systemStack

;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************

c_r13_irq       .long    irqStack
c_r13_kernel    .long    kernelStack
c_r13_abt       .long    abortStack
c_r13_system    .long    systemStack

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
	CPS   0x17
	LDR   sp, c_r13_abt

    ; SET System-Stack
    CPS   0x1F
    LDR   sp, c_r13_system

    ; Disable Interrupts
    MRS   R12, CPSR
    BIC   R12, R12, #192
    MSR   CPSR_cf, R12

    ; Perform all the required initilizations:
    ;  - Process BINIT Table
    ;  - Perform C auto initialization
    ;  - Call global constructors)
    BL    __TI_auto_init

    ; CALL APPLICATION
    BL    ARGS_MAIN_RTN

.end
