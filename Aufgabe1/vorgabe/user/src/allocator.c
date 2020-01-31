#include "blocklist.h"
#include "allocator.h"
#include <stdint.h>

#define NULL (void*)0
#define HEAP_START 0
#define HEAP_SIZE 0
#define BLOCK_INFO_SIZE 3

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
	while (currentBlock != NULL) {
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
		/* clear free bit */
		currentBlock->size &= ~0x2;
		return (void*)&currentBlock->next; /* next unused by fullblocks -> we can use the block from here */ 
		} else {
			currentBlock = currentBlock->next;
			previousBlock = currentBlock;
		}
	}
	return NULL;
}

void insertBlock(struct block* currentBlock, struct block* previousBlock, uint32_t nbytes) {
	struct block* newBlock = (struct block*)((uint32_t*)currentBlock + nbytes/4); /* create new free Block with available space */
	newBlock->size = currentBlock->size - nbytes - BLOCK_INFO_SIZE; /* set new size */
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


void free(void* ptr) {
	struct block* block2Free = (struct block*)((uint32_t*) ptr - 2); /*get address of block info */
	struct block* predecessor = (struct block*)((uint32_t*)block2Free - block2Free->prevSize); /*get predecessor*/
	struct block* successor = (struct block*)((uint32_t*)block2Free + block2Free->size);/*get successor*/
	/*set own free bit*/
	block2Free->size |= 0x2;

	if ((predecessor->size & 0x2) == 0x2) { /* if free bit is set */
		/* merge with predecessor */
		predecessor->size = predecessor->size + block2Free->size + BLOCK_INFO_SIZE; /*add also size of block info*/
		block2Free = predecessor;
		successor->prevSize = block2Free->size;
	}
	if ((successor->size & 0x2) == 0x2) {
		/*merge with successor*/
		block2Free->size = block2Free->size + successor->size + BLOCK_INFO_SIZE;
		struct block* newSucc = (struct block*)((uint32_t)block2Free + block2Free->size);
		newSucc->prevSize = block2Free->size;
	}

	insertFreeList(block2Free);
}

/* Adjusts the firstEmpty pointer if needed, inserts the freed block in the list of free blocks*/
void insertFreeList(struct block* block2Free){
	if (firstEmpty != NULL && firstEmpty > block2Free) {
		block2Free->next = firstEmpty;
		firstEmpty = block2Free;
	} else {
		struct block* currentBlock = firstEmpty;
		while (currentBlock != NULL) {
			if (currentBlock < block2Free && (currentBlock->next == NULL || currentBlock->next > block2Free)) {
				block2Free->next = currentBlock->next;
				currentBlock->next = block2Free;
				return;
			}
			currentBlock = currentBlock->next;
		}
		firstEmpty = block2Free;
		block2Free->next = NULL;
	}
}

void initialiseHeap() {
	struct block* firstBlock = (struct block*) HEAP_START;
	firstBlock->size = HEAP_SIZE - BLOCK_INFO_SIZE;
	firstBlock->size |= 0x2;
	firstBlock->prevSize = 0;
	firstBlock->next = NULL;
}