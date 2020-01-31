#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>
#include "blocklist.h"

typedef uint32_t size_t;

void* malloc(size_t);
uint32_t round2Word(uint32_t);
void insertBlock(struct block*, struct block*, uint32_t);



#endif  /* ALLOCATOR_H */
