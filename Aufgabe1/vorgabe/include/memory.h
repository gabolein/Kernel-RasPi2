#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void initMMU();
void initMMUL1Table(uint32_t*);
void activateMMU(uint32_t*);

uint32_t mmuTable[4096];

#endif
