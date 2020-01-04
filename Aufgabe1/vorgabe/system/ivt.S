
	.global _ivt
	.balign	32
_ivt:
	b	   _reset
	ldr	 pc, __undefined_instruction
	ldr	 pc, __software_interrupt
	ldr	 pc, __prefetch_abort
	ldr	 pc, __data_abort
	ldr	 pc, __not_used
	ldr	 pc, __irq
	ldr	 pc, __fiq

__undefined_instruction:	.word _undefined_instruction
__software_interrupt:		.word _software_interrupt
__prefetch_abort:			.word _prefetch_abort
__data_abort:				.word _data_abort
__not_used:				.word _not_used
__irq:						.word _irq
__fiq:						.word _fiq

	/* Branch Vorbereiten */
_undefined_instruction:
	stmdb   sp!, {r0 - r12}
	stmdb   sp!, {r13 - r15}
	mov	 r0, sp
	bl	  undefined_instruction
	add sp, sp, #4 /* Addressiere das vorher auf dem stack gesicherte LR */
	ldr lr, [sp]	/* Kopiere das gesicherte LR ins LR */
	add sp, sp, #8 /* Addressiere die gesicherten unbanked Register */
	ldmia sp!, {r0 - r12}
	subs pc, lr, #4
_software_interrupt:
	/* Software Interrupt nutzt normalen Registersatz */
	stmdb   sp!, {r0 - r12}
	stmdb   sp!, {r13 - r15}
	mov r0, sp
	bl	software_interrupt
	add sp, sp, #4
	ldr lr, [sp]
	add sp, sp, #8
	ldmia sp!, {r0 - r12}
	subs pc, lr, #4
_prefetch_abort:
	stmdb   sp!, {r0 - r12}
	stmdb   sp!, {r13 - r15}
	mov r0, sp
	bl	prefetch_abort
	b .Lend
_data_abort:
	stmdb   sp!, {r0 - r12}
	stmdb   sp!, {r13 - r15}
	mov	 r0, sp
	bl		data_abort
	add sp, sp, #4 /* Addressiere das vorher auf dem stack gesicherte LR */
	ldr lr, [sp]	/* Kopiere das gesicherte LR ins LR */
	add sp, sp, #8 /* Addressiere die gesicherten unbanked Register */
	ldmia sp!, {r0 - r12}
	subs pc, lr, #4
_not_used:
	nop
_irq:
	stmdb   sp!, {r0 - r12}
	stmdb   sp!, {r13 - r15}
	mov r0, sp
	bl	irq

	add sp, sp, #4 /* Addressiere das vorher auf dem stack gesicherte LR */
	ldr lr, [sp]	/* Kopiere das gesicherte LR ins LR */
	add sp, sp, #8 /* Addressiere die gesicherten unbanked Register */

	ldmia sp!, {r0 - r12}
	subs pc, lr, #4
_fiq:
	stmdb   sp!, {r0 - r12}
	stmdb   sp!, {r13 - r15}
	mov r0, sp
	bl	fiq
	add sp, sp, #4 /* Addressiere das vorher auf dem stack gesicherte LR */
	ldr lr, [sp]	/* Kopiere das gesicherte LR ins LR */
	add sp, sp, #8 /* Addressiere die gesicherten unbanked Register */

	ldmia sp!, {r0 - r12}
	/* Modusbits im SPSR anpassen */
	subs pc, lr, #4
