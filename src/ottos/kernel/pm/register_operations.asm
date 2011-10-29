	.bss _registers, 4, 4
	.global _registers
	.global _save_registers
	.global _load_registers

_load_registers:
	STR r0, registers_a + 0
	STR r1, registers_a + 4
	STR r2, registers_a + 8
	STR r3, registers_a + 12
	STR r4, registers_a + 16
	STR r5, registers_a + 20
	STR r6, registers_a + 24
	STR r7, registers_a + 28
	STR r8, registers_a + 32
	STR r9, registers_a + 36
	STR r10, registers_a + 40
	STR r11, registers_a + 44
	STR r12, registers_a + 48
	STR r13, registers_a + 52

	MOV pc, lr

_save_registers:
    PUSH {r0, r1}
    LDR r0, registers_a

    STR r1, [r0, #4]
    STR r2, [r0, #8]
    STR r3, [r0, #12]
    STR r4, [r0, #16]
    STR r5, [r0, #20]
    STR r6, [r0, #24]
    STR r7, [r0, #28]
    STR r8, [r0, #32]
    STR r9, [r0, #36]
    STR r10, [r0, #40]
    STR r11, [r0, #44]
    STR r12, [r0, #48]
    STR r13, [r0, #52]

    LDR r1, registers_a

    POP {r0}

    STR r0, [r1, #0]

    POP {r1}

    MOV pc, lr

registers_a	.field _registers, 32
