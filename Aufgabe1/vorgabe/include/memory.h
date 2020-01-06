#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define L1_ALIGN (16384)

void initMMU();
void initMMUL1Table(volatile uint32_t*);

__attribute__((aligned (L1_ALIGN))) volatile uint32_t mmuTable[4096];

#endif
