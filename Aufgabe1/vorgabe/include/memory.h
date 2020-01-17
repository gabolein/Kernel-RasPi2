#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>


void initMMU();
void initMMUL1Table(volatile uint32_t*);
void remapUserStack(uint16_t);

extern volatile uint32_t mmuTable[4096];

#endif
