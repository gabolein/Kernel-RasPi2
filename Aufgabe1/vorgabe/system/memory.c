#include <stdint.h>
#include "memory.h"
#include "kio.h"

#define SECTION_BASE_SHIFT_AMOUNT 20
#define SECTION_ENTRY_CODE 0x2
#define AP_LOW 10
#define AP_HIGH 15
#define MMUTABLEBASE 0xc000

#define NO_ACCESS     0b0
#define SYSTEM_ACCESS 0b1
#define SYSTEM_RO     0b101
#define BOTH_RO       0b111
#define RO            0b10
#define FULL_ACCESS   0b11

#define SET_XN 1 << 4
#define SET_PXN 1

extern void _mmuInit();

/* Online geklaut */
uint32_t mmu_section ( uint32_t virtual, uint32_t physical, uint32_t flags )
{
        uint32_t offset = virtual >> 20;
        // plus and or are the same thing here, as MMUTABLEBASE is 14 bit aligned
        uint32_t* entry = MMUTABLEBASE | (offset<<2);

        // mask lower 20 bits of physical address then ORR flags and 0x02 for 1 MiB
        uint32_t physval = (physical & 0xfff00000) | (flags & 0x7ffc) | 0x02;

        *entry = physval;
        return(0);
}
#define CACHEABLE 0x08
#define BUFFERABLE 0x04
/* Klau Ende */

void printTableAddr(uint32_t arg){
        kprintf("Adresse des Tables in Assembly: %x\n", arg);
}

void initMMU() {
        initMMUL1Table(mmuTable);
        _mmuInit();             /* Configures and activates MMU */
}

/* Initializes the MMU L1 Table at the given address */
void initMMUL1Table(volatile uint32_t* table) {
        for(uint32_t i = 0; i < 4096; i++) {
                table[i] = SECTION_ENTRY_CODE | i << SECTION_BASE_SHIFT_AMOUNT | SYSTEM_ACCESS << AP_LOW; /* Sectionentry */
                /* table[i] |= i << SECTION_BASE_SHIFT_AMOUNT; /\* Basisadresse der Section *\/ */
                /* table[i] |= 0b1 << AP_LOW; /\* Zugriffsrechte: Systemzugriff, User mode Tabu LSBs *\/ */
                /* table[i] &= ~(1 << AP_HIGH); /\* Zugriffsrechte MSB *\/ */
        }
        table[2] = SECTION_ENTRY_CODE | 2 << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW; /* Sectionentry */
        table[3] = SECTION_ENTRY_CODE | 3 << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW; /* Thread 0 Stack */ // TODO SET_XNP

        /* table[0] = SECTION_ENTRY_CODE | FULL_ACCESS << AP_LOW; // Kernel Kram */
        /* table[1] = SECTION_ENTRY_CODE | 1 << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW; // User Stacks */
        /* table[2] = SECTION_ENTRY_CODE | 2 << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW | SET_PXN; // User Kram */
        /* kprintf("Content of Kernelspace mmuTable: %x\n", table[0]); */
}


void remapUserStack(uint16_t threadNumber) {
        mmuTable[256] = SECTION_ENTRY_CODE | (3 + threadNumber) << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW | SET_XN;
        asm volatile("mcr p15,0,r1,c8,c7,0"); /* Invalidate TLB Entries */
}
