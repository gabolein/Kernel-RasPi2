    .text
    .extern mmuTable
    .global mmuTableAddr
mmuTableAddr:
    .word mmuTable

    .global printTableAddr
	.global _mmuInit
_mmuInit:
    /* Setze die Adresse der L1 Tabelle */
    mrc p15, 0, r0, c2, c0, 0 // Might not be needed
    /*mov r0, #mmuTableAddr*/
    movw r0, #:lower16:mmuTable
	movt r0, #:upper16:mmuTable
    bl printTableAddr
    mcr p15, 0, r0, c2, c0, 0

    /* Setze das Domain Control Register auf 11 für alle Domains (Manager Mode) */
    mrc p15, 0, r0, c3, c0, 0
    .equ ones, 0xFFFFFFFF
    movw r1, #:lower16:ones
	movt r1, #:upper16:ones
    orr r0, r0, r1
    mcr p15, 0, r0, c2, c0, 0

    /* TTB Control Register auf 16 Bit setzen */
	mrc p15, 0, r0, c2, c0, 2
    movw r1, #0xFFF8 /* Unterste 3 Bits auf Null */
    movt r1, #0xFFFF
    and r0, r0, r1
    mcr p15, 0, r0, c2, c0, 2

    /* Aktiviere die MMU */
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x1
    mcr p15, 0, r0, c1, c0, 0

    bx lr
