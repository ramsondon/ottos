		.if __TI_EABI_ASSEMBLER
			.asg irq_handle_swi, C_INTSWI
			.asg irq_handle, C_INTIRQ
;			.asg handle_fiq, C_INTFIQ
		.else
			.asg _irq_handle_swi, C_INTSWI
			.asg _irq_handle, C_INTIRQ
;			.asg _handle_fiq, C_INTFIQ
		.endif

	.global _c_int00
	.global C_INTSWI	; Software Interrupt
	.global C_INTIRQ	; Interrupt Request

	; interrupt sections

	.sect ".intswi"
	B C_INTSWI ; Software interrupt

	.sect ".intirq"
	B C_INTIRQ ; Interrupt Request
