		.if __TI_EABI_ASSEMBLER
			.asg handle_swi, C_INTSWI
			.asg handle_irq, C_INTIRQ
			.asg handle_fiq, C_INTFIQ
		.else
			.asg _handle_swi, C_INTSWI
			.asg _handle_irq, C_INTIRQ
			.asg _handle_fiq, C_INTFIQ
		.endif

	.global _c_int00
	.global C_INTSWI	; Software Interrupt
	.global C_INTIRQ	; Interrupt Request

	; interrupt sections

	.sect ".intswi"
	B C_INTSWI ; Software interrupt

	.sect ".intirq"
	B C_INTIRQ ; Interrupt Request
