#include <stdint.h>
#include "memory.h"
#include "kio.h"

#define SECTION_BASE_SHIFT_AMOUNT 20
#define SECTION_ENTRY_CODE 2

extern void _mmuInit();

void initMMU() {
        initMMUL1Table(mmuTable);
        _mmuInit();             /* Configures and activates MMU */
}

/* Initializes the MMU L1 Table at the given address */
void initMMUL1Table(uint32_t* table) {
        for(uint32_t i = 0; i < 4096; i++) {
                table[i] = SECTION_ENTRY_CODE; /* Sectionentry */
                /* table[i] |= (i * 4) << SECTION_BASE_SHIFT_AMOUNT; /\* Basisadresse der Section *\/ */
                table[i] |= 0b11 << 10; /* Zugriffsrechte: Vollzugriff */
                kprintf("Inhalt der MMU Table an Stelle %i: %x\n", i, table[i]);
        }

}

void activateMMU(uint32_t* tableStart) {
        asm volatile("mov r1, #0");
        asm volatile("mov r0, %0" : "=r" (tableStart));
        asm volatile("mcr p15, 0, r1, c3, c0, 0"); /* Set Domain Access Control Register to 0 */
        asm volatile("mcr p15, 0, r0, c2, c0, 0"); /* Set Translation Table Base Register to 0 */
        asm volatile("mrc p15, 0, r0, c1, c0, 0");
        asm volatile("orr r0, r0, #0x1");
        asm volatile("mcr p15, 0, r0, c1, c0, 0");
}
