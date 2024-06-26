#include <stdint.h>
#include "memory.h"
#include "kio.h"

#define L1_ALIGN            (16384)
#define PHYSICAL_ADDR_SHIFT 20
#define LAST_USER_DATA_MB   21
#define FIRST_USER_DATA_MB  4
#define WORD                4
#define HEAPS_START_MB      22

__attribute__((aligned (L1_ALIGN))) volatile uint32_t mmuTable[MMU_TABLE_ENTRIES] = {0};

void initMMU() {
        initMMUL1Table(mmuTable);
        _mmuInit();             /* Configures and activates MMU */
}

void setTableEntry(uint32_t virtAddr, uint32_t physAddr, uint32_t flags){
        asm volatile("mcr p15,0,r1,c8,c7,0");                          /* Invalidate TLB Entries */
        mmuTable[virtAddr >> PHYSICAL_ADDR_SHIFT] = (physAddr & SECTION_BASE_BITMASK) | (flags & (~SECTION_BASE_BITMASK)) | SECTION_ENTRY_CODE;
}

void setFaultEntry(uint32_t virtAddr){
        mmuTable[virtAddr >> PHYSICAL_ADDR_SHIFT] = 0;
}

/* Initializes the MMU L1 Table */
void initMMUL1Table() {
        for(uint32_t i = 0; i < MMU_TABLE_ENTRIES; i++) {
                setTableEntry(i << PHYSICAL_ADDR_SHIFT, i << PHYSICAL_ADDR_SHIFT, SYSTEM_ACCESS | SET_XN);
        }

        setTableEntry(0 << PHYSICAL_ADDR_SHIFT, 0 << PHYSICAL_ADDR_SHIFT, SYSTEM_ACCESS);          /* Kernel Text */
        setTableEntry(1 << PHYSICAL_ADDR_SHIFT, 1 << PHYSICAL_ADDR_SHIFT, SYSTEM_ACCESS | SET_XN); /* KBSS, Data, ROData */
        setTableEntry(2 << PHYSICAL_ADDR_SHIFT, 2 << PHYSICAL_ADDR_SHIFT, BOTH_RO       | SET_PXN);/* UText */
        setTableEntry(3 << PHYSICAL_ADDR_SHIFT, 3 << PHYSICAL_ADDR_SHIFT, BOTH_RO       | SET_XN); /* UData, UBSS, UROData */
        for(int i = FIRST_USER_DATA_MB; i <= LAST_USER_DATA_MB; i++){
                setTableEntry(i << PHYSICAL_ADDR_SHIFT, i << PHYSICAL_ADDR_SHIFT, SYSTEM_ACCESS | SET_XN);
        }
        setFaultEntry(257<<PHYSICAL_ADDR_SHIFT);                                                   /* Für Demo von Abgabe 5 */
}

void remapAddressSpace(uint16_t pid) {
        setTableEntry(3<<PHYSICAL_ADDR_SHIFT, (4 + pid * 2)         <<PHYSICAL_ADDR_SHIFT, FULL_ACCESS | SET_XN); /* Data */
        setTableEntry(4<<PHYSICAL_ADDR_SHIFT, (5 + pid * 2)         <<PHYSICAL_ADDR_SHIFT, FULL_ACCESS | SET_XN); /* Stacks */
        setTableEntry(5<<PHYSICAL_ADDR_SHIFT, (HEAPS_START_MB + pid)<<PHYSICAL_ADDR_SHIFT, FULL_ACCESS | SET_XN); /* Heaps */
}

void map1on1() {
        setTableEntry(4<<PHYSICAL_ADDR_SHIFT, 4<<PHYSICAL_ADDR_SHIFT, SYSTEM_ACCESS | SET_XN); /* Data Stacks und Heap werden 1:1 gemappt */
        setTableEntry(3<<PHYSICAL_ADDR_SHIFT, 3<<PHYSICAL_ADDR_SHIFT, SYSTEM_RO     | SET_XN);
        setTableEntry(5<<PHYSICAL_ADDR_SHIFT, 5<<PHYSICAL_ADDR_SHIFT, SYSTEM_ACCESS | SET_XN);
}

/* expects map1on1 */
void nullHeap(uint16_t pid) {
        *((uint32_t*)((HEAPS_START_MB + pid) << PHYSICAL_ADDR_SHIFT)) = 0;
}

void copyUserBlock(uint16_t sourcePID, uint16_t targetPID){
        uint32_t* sourceAddr = (uint32_t*)((FIRST_USER_DATA_MB << PHYSICAL_ADDR_SHIFT) + sourcePID * (2 << PHYSICAL_ADDR_SHIFT));
        uint32_t* targetAddr = (uint32_t*)((FIRST_USER_DATA_MB << PHYSICAL_ADDR_SHIFT) + targetPID * (2 << PHYSICAL_ADDR_SHIFT));
        /* Iteriere über 1 MB und kopiere wortweise */
        for(uint32_t currAddr = 0x0; currAddr < (0x100000); currAddr += WORD){
                *targetAddr = *sourceAddr;
                sourceAddr += 1;
                targetAddr += 1;
        }
}
