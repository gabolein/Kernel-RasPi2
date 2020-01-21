#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define SECTION_BASE_SHIFT_AMOUNT 20
#define SECTION_ENTRY_CODE 0x2
#define SECTION_BASE_BITMASK 0xFFF00000

#define NO_ACCESS     0x0
#define SYSTEM_ACCESS 0x400
#define SYSTEM_RO     0x8400
#define BOTH_RO       0x8C00
#define RO            0x800
#define FULL_ACCESS   0xC00

#define SET_XN (1 << 4)
#define SET_PXN 1


void initMMU();
void initMMUL1Table();
void remapAddressSpace(uint16_t);
void setTableEntry(uint32_t, uint32_t, uint32_t);
void setFaultEntry(uint32_t);
void mapIdleThread();


extern volatile uint32_t mmuTable[4096];



#endif
