
	.global _ivt
	.balign	32
_ivt:
    b       _reset
	ldr     pc, __undefined_instruction
	ldr     pc, __software_interrupt
	ldr     pc, __prefetch_abort
	ldr     pc, __data_abort
	ldr     pc, __not_used
	ldr     pc, __irq
	ldr     pc, __fiq

__undefined_instruction:         .word _undefined_instruction
__software_interrupt:            .word _software_interrupt
__prefetch_abort:                .word _prefetch_abort
__data_abort:                    .word _data_abort
__not_used:                      .word _not_used
__irq:                           .word _irq
__fiq:                           .word _fiq

    /* Branch Vorbereiten */
    /* TODO Register sichern */
    /* TODO IRQ und FIQ abschalten */
_undefined_instruction:
    ldr     r0, =#255
    stmdb   sp, {r0-r15}^ /* Speichere die überlagerten Register (wie sie im unprivilegierten Modus vorliegen), auf dem Stack */
    sub     sp, sp, #64   /* Dekrementiere den SP */
    mov     r0, sp
    bl      undefined_instruction
_software_interrupt:
    /* Software Interrupt nutzt normalen Registersatz */
    push {r0-r12}
	push {r13-r15}
    mov r0, sp
	bl	software_interrupt
_prefetch_abort:
    push {r0-r12}
	push {r13-r15}
	mov r0, sp
	bl	prefetch_abort
_data_abort:
    stmdb   sp, {r0-r15}^ /* Speichere die überlagerten Register (wie sie im unprivilegierten Modus vorliegen), auf dem Stack */
	sub     sp, sp, #64   /* Dekrementiere den SP */
	mov     r0, sp
	bl	    data_abort
_not_used:
    nop
_irq:
        push {r0-r12}
	push {r13-r15}
    mov r0, sp
	bl	irq
_fiq:
    push {r0-r12}
	push {r13-r15}
    mov r0, sp
	bl	fiq
