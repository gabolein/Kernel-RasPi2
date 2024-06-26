
	.globl _reset
_reset:
	b	0x0

	.section .init
	.global _start
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

	/*Stackpointer für Interrupt modes*/
	ldr	r0, =0x20000
	msr	 sp_usr, r0
	ldr	r0, =0x30000
	msr	sp_svc, r0
	ldr	r0, =0x40000
	msr	sp_und, r0
	ldr	r0, =0x50000
	msr	sp_abt, r0
	ldr	r0, =0x60000
	msr	sp_irq, r0
	ldr	r0, =0x70000
	msr	sp_fiq, r0

    /* Adresse der IVT in r0 laden */
	ldr	    r0, =_ivt
	mcr 	p15, 0, r0, c12, c0, 0

	cpsie	i, #0x13
	/* Zu c Code springen */

   	bl	  start_kernel

.global .Lend

.Lend:
	WFI
	b	   .Lend

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
