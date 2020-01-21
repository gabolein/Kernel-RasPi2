#include <stdint.h>
#include "memory.h"
#include "kio.h"

#define L1_ALIGN (16384)

__attribute__((aligned (L1_ALIGN))) volatile uint32_t mmuTable[4096] = {0};

extern void _mmuInit();

void initMMU() {
        initMMUL1Table(mmuTable);
        _mmuInit();             /* Configures and activates MMU */
        kprintf("I'm at the end of initMMU\n");
}

void setTableEntry(uint32_t virtAddr, uint32_t physAddr, uint32_t flags){
        mmuTable[virtAddr >> 20] = (physAddr & SECTION_BASE_BITMASK) | (flags & (~SECTION_BASE_BITMASK)) | SECTION_ENTRY_CODE;
}

void setFaultEntry(uint32_t virtAddr){
        mmuTable[virtAddr >> 20] = 0;
}

/* Initializes the MMU L1 Table at the given address */
void initMMUL1Table() {

        for(uint32_t i = 0; i < 4096; i++) {
                setTableEntry(i<<20, i<<20, SYSTEM_ACCESS | SET_XN);
        }

        setTableEntry(0<<20, 0<<20, SYSTEM_ACCESS);              /* Kernel Text, ROData */
        setTableEntry(1<<20, 1<<20, SYSTEM_ACCESS | SET_XN); /* KBSS, Data */
        setTableEntry(2<<20, 2<<20, BOTH_RO | SET_PXN);      /* UText, UROData */
        setTableEntry(3<<20, 3<<20, BOTH_RO | SET_XN);       /* UData, UBSS (erstmal nur RO für den IDLE Thread) */

        setFaultEntry(257<<20);                              /* Für Demo */
        kprintf("I'm still alive\n");
}

void remapAddressSpace(uint16_t pid) {
        if (pid == 8) {
                mapIdleThread;
                return;
        }
        asm volatile("mcr p15,0,r1,c8,c7,0");                          /* Invalidate TLB Entries */
        setTableEntry(3<<20, (4 + pid * 2)<<20, FULL_ACCESS | SET_XN);    /* Data */
        setTableEntry(4<<20, (5 + pid * 2)<<20, FULL_ACCESS | SET_XN);    /* Stacks */
}

void mapIdleThread() {
        /* Map the Idle Stack located at 0x14 to 0x400000 */
        asm volatile("mcr p15,0,r1,c8,c7,0");                             /* Invalidate TLB Entries */
        setTableEntry(3<<20, 3<<20, BOTH_RO | SET_XN);       /* UData, UBSS (erstmal nur RO für den IDLE Thread) */
        setTableEntry(4<<20, 20<<20, FULL_ACCESS | SET_XN);  /* IDLE Stack */
}
