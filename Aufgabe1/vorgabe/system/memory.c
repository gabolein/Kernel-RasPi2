#include <stdint.h>
#include "memory.h"
#include "kio.h"

#define SECTION_BASE_SHIFT_AMOUNT 20
#define SECTION_ENTRY_CODE 0x2
#define AP_LOW 10
#define AP_HIGH 15
#define MMUTABLEBASE 0xc000

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
        //mmu_section(0x8000, 0x8000, CACHEABLE | BUFFERABLE);
        kprintf("Adresse des Tables in C: %x\n", mmuTable);
        kprintf("mmu_section Eintrag 0: %x\n", mmuTable[0]);
        _mmuInit();             /* Configures and activates MMU */
}

/* Initializes the MMU L1 Table at the given address */
void initMMUL1Table(volatile uint32_t* table) {
        for(uint32_t i = 0; i < 4096; i++) {
                table[i] = SECTION_ENTRY_CODE; /* Sectionentry */
                table[i] |= i << SECTION_BASE_SHIFT_AMOUNT; /* Basisadresse der Section */
                table[i] |= 0b1 << AP_LOW; /* Zugriffsrechte: Vollzugriff LSBs */
                table[i] &= ~(1 << AP_HIGH); /* Zugriffsrechte MSB */
        }
        table[1] = SECTION_ENTRY_CODE | 1 << SECTION_BASE_SHIFT_AMOUNT | 0b11 << AP_LOW;
        kprintf("Inhalt der MMU Table an erster Stelle: %x\n", mmuTable[0]);
        kprintf("Inhalt der MMU Table an letzter Stelle: %x\n", mmuTable[4095]);
}
