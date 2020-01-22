#include "io.h"
#include <stdint.h>
#include "swiInterface.h"
#include "testThread.h"
#include "threadUtil.h"
#include "user_thread.h"


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

/* void test(void* arg){ */
/*         char* myChar = (char*)arg; */
/*         asm volatile("push {%0}" :: "r" (*myChar)); */
/*         sleep(300); */
/*         char meinChar = 0; */
/*         asm volatile("pop {%0}" : "+r" (meinChar)); */
/*         printf("meinChar ist: %c\n", meinChar); */
/* } */

void spawner() {
        while(1) {
                volatile char c = getChar();
                if (c) {
                        uint32_t number = (uint32_t) c;
                        newProcess(&demonstration6, &number, 1);
                }
        }
}

void demonstration6(void* arg){
        uint16_t threadID = 0;
        asm volatile("mov %0, r1": "+r" (threadID));
        char* myChar = (char*)arg;
        charStorage = *myChar;
        newThread(&demonstration6Thread, NULL, 0);
        sleep(100);             /*  */
        newThread(&demonstration6Thread, NULL, 0);
        sleep(100);
        volatile uint32_t localCounter = 0;
        while(globalCounter < COUNTER_LIMIT){
                globalCounter++;
                localCounter++;
                printf("%c:%u (%u:%u)\n", charStorage, globalCounter, threadID, localCounter);
                sleep(100);
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
                sleep(100);
        }
}

void entertainer() {

        asm volatile("mov r0, #123");
        asm volatile("push {r0}");
        asm volatile("mov r0, #0");
        sleep(300);
        uint32_t holdup = 0;
        uint32_t sp = 0;
        asm volatile("mov %0, sp": "+r" (sp));
        asm volatile("mov %0, r0": "+r" (holdup));
        printf("Mein Stackpointer ist: %x\n", sp);
        printf("R0 enthält (vor pop): %u\n", holdup);
        asm volatile("pop {r0}");
        asm volatile("mov %0, r0": "+r" (holdup));
        printf("R0 enthält (nach pop): %u\n", holdup);

        /* while(1) { */

        /*         sleep(3000); */
        /*         char* string = "Hallo Freunde!\n"; */
        /*         newThread(&printer, string, 15); */
        /* } */
}

void printer(void* arg) {
        printf("hello i am the printer this is my song\n");
        char* string = (char*)arg;
        printf(string);
}

void demonstration5(void* arg) {
        printf("ICh bin die Demo mit arg %c\n", *(char*)arg);
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
