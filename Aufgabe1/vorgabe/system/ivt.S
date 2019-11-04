
        
	.global _ivt	
	.balign	32
_ivt:	        b       _reset
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
_undefined_instruction:
	b	undefined_instruction
_software_interrupt:
        /* Software Interrupt nutzt normalen Registersatz */
	b	software_interrupt
_prefetch_abort:
	b	prefetch_abort
_data_abort:    
	b	data_abort
_not_used:
        nop
_irq:
	b	irq
_fiq:
	b	fiq
        
