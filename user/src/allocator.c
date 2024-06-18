#include "blocklist.h"
#include "allocator.h"
#include <stdint.h>
#include <io.h>

#define NULL       (void*)0
#define HEAP_START (void*)0x500000
#define HEAP_SIZE  0x100000 - 4
#define USED_BLOCK_INFO_SIZE 2
#define WORD_SIZE  4
#define FREE_BIT   (0b10)
#define TOP_BIT    (0b1)
#define STATUS_BITS (0b11)

struct block* firstEmpty = 0;

/* Rounds the number of Bytes to a word aligned number */
uint32_t round2Word(uint32_t numBytes) {
        if((numBytes & 0b11)){
                numBytes &= ~(0b11);
                numBytes += FREE_BIT;
        }
        return numBytes;
}

void* malloc(size_t nbytes) {
        if((*(uint32_t*)HEAP_START) == 0) {
                /* Heap not initialized yet */
                initialiseHeap();
        }
        nbytes = round2Word(nbytes);
        struct block* currentBlock = firstEmpty;
        struct block* previousBlock = NULL;
        while (currentBlock != NULL) {
                if ((currentBlock->size & ~STATUS_BITS) >= nbytes) { /* update linked lists */
                        if ((currentBlock->size & ~STATUS_BITS) > nbytes) {
                                insertBlock(currentBlock, previousBlock, nbytes);
                        } else {
                                if (previousBlock != NULL) {
                                        if (currentBlock == firstEmpty) {
                                                firstEmpty = currentBlock->next; /* TODO Block existiert noch nicht */
                                        }
                                        previousBlock->next = currentBlock->next;
                                        currentBlock->next = NULL;
                                }
                        }
                        /* clear free bit */
                        currentBlock->size &= ~(FREE_BIT);
                        return (void*)&(currentBlock->next); /* next is unused by used blocks -> we can use the block from here */
                } else {
                        currentBlock = currentBlock->next;
                        previousBlock = currentBlock;
                }
        }
        printf("malloc couldn't alloc (a block of sufficient size)\n");
        return NULL;
}

void insertBlock(struct block* currentBlock, struct block* previousBlock, uint32_t nbytes) {
        struct block* newBlock = (struct block*)((uint32_t*)currentBlock + nbytes/4 + USED_BLOCK_INFO_SIZE); /* create new free Block with available space */
        newBlock->size = ((currentBlock->size & ~STATUS_BITS) - nbytes) - USED_BLOCK_INFO_SIZE * WORD_SIZE; /* set new size */
        newBlock->size |= FREE_BIT;
        printf("newBlock->size: %u\n", newBlock->size & ~STATUS_BITS);
        printf("size vom currentBlock vor blyat %u\n", currentBlock->size);
        currentBlock->size = nbytes;
        printf("size vom currentBlock %u\n", currentBlock->size);
        newBlock->prevSize = currentBlock->size & ~STATUS_BITS; /*set size of predecessor */
        newBlock->next = currentBlock->next; /* inherit the successor*/
        if(currentBlock == firstEmpty) {
                firstEmpty = newBlock;
        }
        currentBlock->next = NULL;
        printf("Berechnung: %u \n", ((uint32_t)newBlock + (newBlock->size & ~STATUS_BITS)));
        if(((uint32_t)newBlock + (newBlock->size & ~STATUS_BITS) + 4) < 0x600000){
                struct block* nextBlock = (struct block*)((uint32_t*)newBlock + (newBlock->size & ~STATUS_BITS)/4); /* get the next block and adjust the predecessor size */
                nextBlock->prevSize = newBlock->size & ~STATUS_BITS;
        }
        if (previousBlock != NULL) { /* link the previous free block to the new block */
                previousBlock->next = newBlock;
        }
}


void free(void* ptr) {
        if((*(uint32_t*)HEAP_START) == 0) {
                /* Heap not initialized yet */
                return;
        }
        /* Check whether ptr is in bounds of heap region */
        if(!((ptr > HEAP_START) && (ptr <= HEAP_START + HEAP_SIZE))){
                return;
        }

        struct block* block2Free  = (struct block*)((uint32_t*)ptr - USED_BLOCK_INFO_SIZE); /*get address of block info */

		/* check if block2Free actually a block */
		struct block* currentBlock = (struct block*) HEAP_START;
        while(currentBlock) {
        		if(currentBlock == block2Free) {
        			break;
        		}
                currentBlock = (struct block*)((uint32_t)currentBlock + USED_BLOCK_INFO_SIZE * 4 + currentBlock->size);
                if((void*)currentBlock >= HEAP_START + HEAP_SIZE){
                		printf("Given pointer not start of block\n");
                        return;
                }
        }



        struct block* predecessor = (struct block*)((uint32_t*)block2Free - block2Free->prevSize / 4 - USED_BLOCK_INFO_SIZE); /*get predecessor*/
        struct block* successor   = (struct block*)((uint32_t*)block2Free + USED_BLOCK_INFO_SIZE + (block2Free->size & ~STATUS_BITS) / 4);/*get successor*/
        printf("Successor is %x, block2free soße is %u\n", successor, block2Free->size);

        if(block2Free->size & FREE_BIT) {
                printf("Block already freed\n");
                return;
        }

        block2Free->size |= FREE_BIT;
        printf("a\n");
        /* Check if predecessor is free */
        if ((predecessor->size & FREE_BIT) && (block2Free->prevSize > 0)) {
                printf("b\n");
                /* merge with predecessor */
                predecessor->size += (block2Free->size & ~STATUS_BITS) + USED_BLOCK_INFO_SIZE; /*add also size of block info*/
                printf("c\n");
                block2Free = predecessor;
                successor->prevSize = (block2Free->size & ~STATUS_BITS);
                printf("d\n");
        }
        printf("a,5\n");
        if (successor->size & FREE_BIT) {
                printf("e\n");
                /* merge with successor */
                block2Free->size += (successor->size & ~STATUS_BITS) + USED_BLOCK_INFO_SIZE * WORD_SIZE;
                printf("f\n");
        }
        printf("ananas\n");
        if(((uint32_t)block2Free + (block2Free->size & ~STATUS_BITS) + USED_BLOCK_INFO_SIZE * WORD_SIZE) < (uint32_t)(HEAP_SIZE + HEAP_START)) {
                struct block* newSucc = (struct block*)((uint32_t)block2Free + (block2Free->size & ~STATUS_BITS) + USED_BLOCK_INFO_SIZE * 4);
                newSucc->prevSize = block2Free->size & ~STATUS_BITS;
        }
        printf("h\n");
        insertFreeList(block2Free);

        printf("i\n");
}

/* Update the firstEmpty pointer if needed, inserts the freed block in the list of free blocks*/
void insertFreeList(struct block* block2Free){
        if ((firstEmpty == NULL) || (firstEmpty > block2Free)) {
                block2Free->next = firstEmpty;
                firstEmpty = block2Free;
                return;
        }

        struct block* currentBlock = firstEmpty;
        while (currentBlock != NULL) {

                if ((currentBlock < block2Free) && ((currentBlock->next == NULL) || (currentBlock->next > block2Free))) {
                        block2Free->next = currentBlock->next;
                        currentBlock->next = block2Free;
                        return;
                }
                currentBlock = currentBlock->next;
        }

        printf("insertFreeList broken \n");
}

void initialiseHeap() {
        firstEmpty = (struct block*) HEAP_START;
        firstEmpty->size = HEAP_SIZE - USED_BLOCK_INFO_SIZE * WORD_SIZE;
        firstEmpty->size |= FREE_BIT;
        firstEmpty->prevSize = 0;
        firstEmpty->next = NULL;
}

void printHeap() {
        printf("---------- Heap Print ----------\n");
        if(!*((uint32_t*) HEAP_START)){
                printf("Heap is empty!\n");
                printf("----------  Heap END  ----------\n");

                return;
        }

        struct block* currentBlock = (struct block*) HEAP_START;
        int counter = 0;
        while(currentBlock) {
                counter++;
                int size = currentBlock->size;
                int freeStatus = currentBlock->size & FREE_BIT;
                int previousSize = currentBlock->prevSize;

                printf("Block %u at address %x\n", counter, (uint32_t)currentBlock);
                printf("Size: %u\n", size);
                printf("Previous size: %u\n", previousSize);
                if(freeStatus){
                        printf("Free: Yes\n");
                        printf("Next Address: %x\n", currentBlock->next);
                } else {
                        printf("Free: No \n");
                }

                currentBlock = (struct block*)((uint32_t)currentBlock + USED_BLOCK_INFO_SIZE * 4 + size);
                if((void*)currentBlock >= HEAP_START + HEAP_SIZE){
                        break;
                }
        }
        printf("----------  Heap END  ----------\n");
}
