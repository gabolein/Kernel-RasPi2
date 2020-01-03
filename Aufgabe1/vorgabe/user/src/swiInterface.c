#include "swiInterface.h"
#include <stdint.h>

/* put given character into r1, then call software interrupt 1*/
void putChar(char x) {
        asm volatile("mov r1, %0"::"r" (x));
        asm volatile("mov r7, #0");
        asm volatile("swi #0");
}

/* calls software interrupt, then returns content of r0*/
char getChar() {
        asm volatile("mov r7, #1");
        asm volatile("swi #1");
        uint32_t holder = 0;
        asm volatile("mov %0, r0": "+r" (holder));
        return (char)holder;
}

/* put arguments in r1-r3, call software interrupt */
void newThread(void (*func)(void *), const void * args, uint32_t args_size) {
	asm volatile("mov r1, %0"::"r" (func));
	asm volatile("mov r2, %0"::"r" (args));
	asm volatile("mov r3, %0"::"r" (args_size));
        asm volatile("mov r7, #2");
        asm volatile("swi #2");
}

/* call software interrupt, no exit code */
void exit(){
        asm volatile("mov r7, #3");
        asm volatile ("swi #3");
}

void sleep(uint32_t sleepTime) {
        asm volatile("mov r7, #4");
        asm volatile("swi #4");
}
