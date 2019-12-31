#include "swiInterface.h"
#include <stdint.h>

/* put given character into r1, then call software interrupt 1*/
void putChar(char x) {
        asm volatile("mov r1, %0"::"r" (x));
        asm volatile("mov r0, #0");
        asm volatile("swi #0");
}

/* calls software interrupt, then returns content of r0*/
char getChar() {
        asm volatile("mov r0, #1");
        asm volatile("swi #1");
        uint32_t holder = 0;
        asm volatile("mov %0, r0": "+r" (holder));
        return (char)holder;
}

/* put arguments on stack, call software interrupt */
uint8_t newThread(void (*func)(void *), const void * args, uint32_t args_size) {
        asm volatile("ldmia sp!, {r0 - r2}");
        asm volatile("mov r0, #2");
        asm volatile("swi #2");
        return 1;
}

/* write exit code in register, call software interrupt */
void exit(uint32_t exitCode){
        asm volatile("mov r0, #3");
        asm volatile ("swi #3");
}

void sleep(uint32_t sleepTime) {
        asm volatile("mov r0, #4");
        asm volatile("swi #4");
}
