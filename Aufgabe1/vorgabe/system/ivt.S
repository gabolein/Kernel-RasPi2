
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
    push {r0-r12}
    str PC, [sp, #-4]!
    b   undefined_instruction
_software_interrupt:
    /* Software Interrupt nutzt normalen Registersatz */
    push {r0-r12}
    str PC, [sp, #-4]!
	b	software_interrupt
_prefetch_abort:
    push {r0-r12}
	str SP, [sp, #-4]!
    str LR, [sp, #-4]!
    str PC, [sp, #-4]!


	b	prefetch_abort
_data_abort:
    push {r0-r12}
    str PC, [sp, #-4]!
	b	data_abort
_not_used:
    nop
_irq:
    push {r0-r12}
    str PC, [sp, #-4]!
	b	irq
_fiq    :
    push {r0-r12}
    str PC, [sp, #-4]!
	b	fiq
