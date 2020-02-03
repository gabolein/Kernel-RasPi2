#include "io.h"
#include <stdint.h>
#include "swiInterface.h"
#include "testThread.h"
#include "threadUtil.h"
#include "user_thread.h"
#include "allocator.h"

#define COMPUTATION_LEN 300000
#define AMOUNT_CHARS 3
#define CAPITAL_LETTER_LIM 97
#define SLEEP_TIME 5000
#define KERNEL_DATA_ADDR  (uint32_t*)0x8000
#define KERNEL_TEXT_ADDR  (uint32_t*)0x8000
#define KERNEL_STACK_ADDR (uint32_t*)0x30000
#define USER_TEXT_ADDR    (uint32_t*)0x200000
#define UNASSIGNED_ADDR   (uint32_t*)0x10100000
#define MAGIC_NUMBER 69
#define NULL (void*)0
#define COUNTER_LIMIT 10

#define UART_BASE_ADDR (0x7E201000 - 0x3F000000)
#define UART_DR        ((uint32_t *) UART_BASE_ADDR)

static volatile char charStorage = 0;
static volatile uint32_t globalCounter = 0;

void user_thread(void* arg) {
        char receivedChar = *(char*)arg;
        for (int i = 0; i < AMOUNT_CHARS; i++) {
                putChar(receivedChar);
                if (receivedChar < CAPITAL_LETTER_LIM) {
                        blockFunc();
                } else {
                        sleep(SLEEP_TIME);
                }
        }
}

void spawner() {
        while(1) {
                volatile char c = getChar();
                if (c == 'm') {
                        newProcess(&mallocDemo, NULL, 0);
                }
        }
}

void mallocDemo() {
        uint32_t *ptrs = malloc(0x20);
        /* Iteriere endlos über ptrs Array */
        while(1){
                for(int i = 0; i < 64; i++){
                        free(ptrs[i]);
                        /* ptrs[i] auf 4 Bit reduzieren */
                        uint8_t X = (ptrs[i] & 0xF) ^ ((ptrs[i] & (0xF << 4)) >> 4);
                        for(int j = 8; j <= 28; j += 4){
                                X ^= (ptrs[i] & (0xF << j)) >> j;
                        }
                        if(!X){
                                X = 1;
                        } else if(X == 0xF){
                                free(ptrs[i]);
                        }
                        ptrs[i] = malloc(X ∗ 256 + X ∗ 16 + X);
                }
        }
}

void demonstration6(void* arg){
        uint16_t threadID = 0;
        asm volatile("mov %0, r1": "+r" (threadID));
        char* myChar = (char*)arg;
        charStorage = *myChar;
        newThread(&demonstration6Thread, NULL, 0);
        newThread(&demonstration6Thread, NULL, 0);
        volatile uint32_t localCounter = 0;
        while(globalCounter < COUNTER_LIMIT){
                globalCounter++;
                localCounter++;
                printf("%c:%u (%u:%u)\n", charStorage, globalCounter, threadID, localCounter);
                sleep(1);
        }
}

void demonstration6Thread(){
        uint16_t threadID = 0;
        asm volatile("mov %0, r1": "+r" (threadID));
        volatile uint32_t localCounter = 0;
        while(globalCounter < COUNTER_LIMIT){
                globalCounter++;
                localCounter++;
                printf("%c:%u (%u:%u)\n", charStorage, globalCounter, threadID, localCounter);
                sleep(1);
        }
}


void demonstration5(void* arg) {
        printf("Ich bin die Demo mit arg %c\n", *(char*)arg);
        volatile uint32_t* addr;
        volatile uint32_t holder;
        switch(*(char*)arg) {
        case 'n':
                addr = NULL;
                holder = *addr;
                break;
        case 'p':
                addr = NULL;
                asm volatile("mov pc, %0":: "r" (addr));
                break;
        case 'd':
                addr = KERNEL_DATA_ADDR;
                holder = *addr;
                break;
        case 'k':
                addr = KERNEL_TEXT_ADDR;
                holder = *addr;
                break;
        case 'K':
                addr = KERNEL_STACK_ADDR;
                holder = *addr;
                break;
        case 'g':
                addr = UART_DR;
                holder = *addr;
                break;
        case 'c':
                addr = USER_TEXT_ADDR;
                *addr = MAGIC_NUMBER;
                break;
        case 's':
                holder = 0;
                while(1) {
                        asm volatile("push {%0}":: "r" (holder));
                        holder++;
                }
                break;
        case 'u':
                addr = UNASSIGNED_ADDR;
                holder = *addr;
                break;
        case 'x':
                asm volatile("mov pc, sp");
                break;
        default:
                printf("\nUndefined command. You have failed. I, now, have to die. Try again.\n");
                break;
        }
}
