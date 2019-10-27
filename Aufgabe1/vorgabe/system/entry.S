	.section .ivt
	.global _ivt
_ivt:     
	        /*IVT*/
	        b       reset
	        ldr     pc, _undefined_instruction
	        ldr     pc, _software_interrupt
	        ldr     pc, _prefetch_abort
	        ldr     pc, _data_abort
	        ldr     pc, _not_used
	        ldr     pc, _irq
	        ldr     pc, _fiq

	        _undefined_instruction:         .word undefined_instruction
	_software_interrupt:            .word software_interrupt
	_prefetch_abort:                .word prefetch_abort
	_data_abort:                    .word data_abort
	_not_used:                      .word not_used
	_irq:                           .word ird
	_fiq:                           .word fiq

	





        .section .init
        .global _start

/*Hoffentlich funktioniert das so*/
reset:  
_start:
        /* Erkennen ob Hypervisor Mode aktiv */
	mrs r0, cpsr
	and r0, r0, #0x1F
	mov r1, #0x1A	//Modebits = 0x1A = Hypervisor Mode
	cmp r0, r1		//Vergleichen und Statusbits setzen für bedingte Sprünge
	
	/* Wenn Hypemode erkannt: abschalten (sprung nach _exitHyper)
	 * wenn nicht erkannt: dann weitermachen (weiterlaufen nach _bsprak)
	 */
	beq _exitHyper

	/* Qemu startet immer alle 4 Kerne
	 * Wir lassen alle anderen Kerne endless loopen
	 */
_loopCores:
	/* Id des Cpu Cores Abfragen */
	mrc p15, 0, r0, c0, c0, 5
	/* Falls nicht core 0 -> Endless loop */
	tst r0, #3
	bne .Lend

_bsprak:
	/* Wir brauchen einen validen Stack für Qemu.
	 * Auf dem Pi wurde dieser zwar bereits durch U-Boot
	 * gesetzt, schadet aber auch nicht
	 */
	ldr	sp,=0x20000
	/* Zu c Code springen */
        /* Adresse der IVT in r0 laden */
        ldr     r0,=_ivt
   	bl      start_kernel
        
.Lend:  
	WFI
	b       .Lend

_exitHyper:

	/* Rücksprungadresse ins Hypervisor Rücksprungregister schreiben.
	 * Dies ist das Label _bsprak für die normale Programmausfürung.
	 */
	ldr lr, =_loopCores
	msr ELR_hyp, lr
	
	/* Der Hypervisor mode ist auch "nur" eine etwas kompliziertere Exception.
	 * Also springen wir aus dieser zurück.
	 * (Rücksprunge in Aufgabe 2 sind anders zu lösen!).
	 * Dazu zunächst das CPSR auslesen und die Modebits zum Supervisor ändern.
	 * Danach dies in das SPSR für den Rücksprung schreiben.
	 */
	 
	mrs r0, cpsr
	bic r0, r0, #0x1F
	orr r0, r0, #0x13	//Supervisormode
	msr spsr_hyp, r0
	eret

        
