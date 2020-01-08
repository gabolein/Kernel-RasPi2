    .text
    .extern mmuTable
    .global printTableAddr
	.global _mmuInit
_mmuInit:
    mov r1, #0
    // invalidate TLB entries
	mcr p15,0,r1,c8,c7,0
	// data synchronisation barrier
	//mcr p15,0,r1,c7,c10,4

    /* Setze die Adresse der L1 Tabelle */
    movw r0, #:lower16:mmuTable
	movt r0, #:upper16:mmuTable
    mcr p15, 0, r0, c2, c0, 0   /* <--- Table Adresse setzen*/
    /* ---------- */

    /* Setze das Domain Control Register auf 11 für alle Domains (Manager Mode) */
    /*
    mrc p15, 0, r0, c3, c0, 0
    .equ ones, 0xFFFFFFFF
    movw r1, #:lower16:ones
	movt r1, #:upper16:ones
    orr r0, r0, r1
    */
    mov r0, #0x1 //Client Mode für Domain 0
    mcr p15, 0, r0, c3, c0, 0 /* <--- Domains konfigurieren */
	/* ---------- */

    /* TTB Control Register auf 16 KB setzen */
	mov r0, #0x0 /* Neu */
    mcr p15, 0, r0, c2, c0, 2 /* <--- Auf 16 KB Mode setzen  */

    /* Aktiviere die MMU */
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x1
    ldr r1, =0x1004 /* Caches ausmachen */
    bic r0, r1
    mcr p15, 0, r0, c1, c0, 0 /* <--- MMU einschalten */

	mov pc, lr // Fertig
