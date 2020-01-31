#include "blocklist.h"
#include "allocator.h"
#include <stdint.h>

#define NULL (void*)0

struct block* firstEmpty;

uint32_t round2Word(uint32_t num) {
	num--;
	num |= num >> 1;
	num |= num >> 2;
	num |= num >> 4;
	num |= num >> 8;
	num |= num >> 16;
	num++;
	return num;
}

void* malloc(size_t nbytes) {
	nbytes = round2Word(nbytes);
	struct block* currentBlock = firstEmpty;
	struct block* previousBlock = NULL;
	do {
		if (currentBlock->size >= nbytes) { /* adjust links of lists */
			if (currentBlock == firstEmpty) {
				firstEmpty = currentBlock->next;
			}
			if (currentBlock->size > nbytes) {
				insertBlock(currentBlock, previousBlock, nbytes);
			} else {
				if (previousBlock != NULL) {
					previousBlock->next = currentBlock->next;
					currentBlock->next = NULL;
				}
			}
		return (void*)&currentBlock->next; /* next unused by fullblocks -> we can use the block from here */ 
		} else {
			currentBlock = currentBlock->next;
			previousBlock = currentBlock;
		}
	} while (currentBlock->next != NULL);
	return NULL;
}


		
		

void insertBlock(struct block* currentBlock, struct block* previousBlock, uint32_t nbytes) {
	struct block* newBlock = (struct block*)((uint32_t*)currentBlock + nbytes/4); /* create new free Block with available space */
	newBlock->size = currentBlock->size - nbytes; /* set new size */
	currentBlock->size = nbytes;				
	newBlock->prevSize = currentBlock->size; /*set size of predecessor */
	newBlock->next = currentBlock->next; /* inherit the successor*/
	
	currentBlock->next = NULL;
	
	struct block* nextBlock = (struct block*)((uint32_t*)newBlock + newBlock->size/4); /* get the next block and adjust the predecessor size */
	nextBlock->prevSize = newBlock->size;

	if (previousBlock != NULL) { /* link the previous free block to the new block */
		previousBlock->next = newBlock;
	}
}