		.if __TI_EABI_ASSEMBLER
			.asg c_intIRQ, C_INTIRQ
			.asg c_intSWI, C_INTSWI
		.else
			.asg _c_intIRQ, C_INTIRQ
			.asg _c_intSWI, C_INTSWI
		.endif

	.global _c_int00
	.global C_INTIRQ
	.global C_INTSWI	; Software Interrupt

	.sect ".intswi"
	B C_INTSWI ; Software interrupt

	.sect ".intirq"
	B C_INTIRQ ; Interrupt Request
