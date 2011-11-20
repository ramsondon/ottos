	.asg _irq_handle_swi, C_INTSWI
	.asg _irq_handle, C_INTIRQ

	.global _c_int00
	.global C_INTSWI
	.global C_INTIRQ

	.sect ".intvecs"
		B _c_int00 ; Reset Interrupt
		.word 0 ; Undefined Instructions Interrupt
		B C_INTSWI ; Software Interrupt
		.word 0 ; Prefetch Abort Interrupt
		.word 0 ; Data Abort Interrupt
		.word 0 ; Unused
		B C_INTIRQ, ; IRQ Interrupt
		.word 0 ; FIQ Interrupt
