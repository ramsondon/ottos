	.asg _irq_handle_udef, C_INTUDEF
	.asg _irq_handle_swi, C_INTSWI
	.asg _irq_handle_pabt, C_INTPABT
	.asg _irq_handle_dabt, C_INTDABT
	.asg _irq_handle, C_INTIRQ

	.global _c_int00
	.global C_INTUDEF	; Undefined Exception
	.global C_INTSWI	; Software Interrupt
	.global C_INTPABT	; Prefetch abort
	.global C_INTDABT	; Data abort
	.global C_INTIRQ	; Interrupt Request

	; interrupt sections
	.sect ".intvecs"
	B _c_int00 ; Reset Interrupt
	B C_INTUDEF ; Undefined Exception
	B C_INTSWI ; Software Interrupt
	B C_INTPABT ; Prefetch Abort Interrupt
	B C_INTDABT ; Data Abort Interrupt
	.word 0 ; Unused
	B C_INTIRQ, ; IRQ Interrupt
	.word 0 ; FIQ Interrupt
