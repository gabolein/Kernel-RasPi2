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
#define KERNEL_DATA_ADDR 0
#define KERNEL_TEXT_ADDR 0
#define KERNEL_STACK_ADDR 0
#define USER_TEXT_ADDR 0
#define UART_DR 0
#define UNASSIGNED_ADDR 0
#define MAGIC_NUMBER 69
#define NULL (void*)0

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
                if (c) {
                        uint32_t number = (uint32_t) c;
                        newThread(&demonstration5, &number, 1);
                }
        }
}

void entertainer() {
        while(1) {
                sleep(3000);
                char* string = "Hallo Freunde!\n";
                newThread(&printer, string, 15);
        }
}

void printer(void* arg) {
        printf("hello i am the printer this is my song\n");
        char* string = (char*)arg;
        printf(string);
}

void demonstration5(void* arg) {
        uint32_t* addr;
        uint32_t holder;
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
