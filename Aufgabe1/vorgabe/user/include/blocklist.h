#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <stdint.h>

struct block {
	uint32_t size;
	uint32_t prevSize;
	struct block* next;         /* next free block */
};


#endif  /* BLOCKLIST_H */
