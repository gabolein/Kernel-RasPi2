#include <stdint.h>
#include "memory.h"
#include "kio.h"

#define SECTION_BASE_SHIFT_AMOUNT 20
#define SECTION_ENTRY_CODE 0x2
#define AP_LOW 10
#define AP_HIGH 15

#define NO_ACCESS     0b0
#define SYSTEM_ACCESS 0b1
#define SYSTEM_RO     0b101
#define BOTH_RO       0b111
#define RO            0b10
#define FULL_ACCESS   0b11

#define SET_XN 1 << 4
#define SET_PXN 1

#define L1_ALIGN (16384)

__attribute__((aligned (L1_ALIGN))) volatile uint32_t mmuTable[4096] = {0};

extern void _mmuInit();

void initMMU() {
        initMMUL1Table(mmuTable);
        _mmuInit();             /* Configures and activates MMU */
}

/* Initializes the MMU L1 Table at the given address */
void initMMUL1Table(volatile uint32_t* table) {
        for(uint32_t i = 0; i < 4096; i++) {
                table[i] = SECTION_ENTRY_CODE | i << SECTION_BASE_SHIFT_AMOUNT | SYSTEM_ACCESS << AP_LOW | SET_XN; /* Sectionentry */
        }
        mmuTable[257] = 0;
        table[0] = SECTION_ENTRY_CODE | 0 << SECTION_BASE_SHIFT_AMOUNT | SYSTEM_ACCESS << AP_LOW; /* Kernel Text */
        table[1] = SECTION_ENTRY_CODE | 1 << SECTION_BASE_SHIFT_AMOUNT | SYSTEM_ACCESS << AP_LOW | SET_XN; /* Kernel Data */
        table[2] = SECTION_ENTRY_CODE | 2 << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW | SET_XN; /* User Data  */
        table[3] = SECTION_ENTRY_CODE | 3 << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW | SET_PXN; /* User Text  */
}
/* ------ 0x400000 User Stacks ab hier */

/* ------ 0x300000 User Text */

/* ------ 0x200000 User Data */

/* ------ 0x10000 Kernel Data */

/* ------ 0x00000 (0x8000) Kernel Text */

void remapUserStack(uint16_t threadNumber) {
        asm volatile("mcr p15,0,r1,c8,c7,0"); /* Invalidate TLB Entries */
        mmuTable[256] = SECTION_ENTRY_CODE | (4 + threadNumber) << SECTION_BASE_SHIFT_AMOUNT | FULL_ACCESS << AP_LOW | SET_XN;
}
