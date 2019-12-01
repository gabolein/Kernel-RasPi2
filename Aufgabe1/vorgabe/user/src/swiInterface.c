#include "swiInterface.h"
#include <stdint.h>

/* put given character into r0, then call software interrupt 1*/
void putChar(char x) {
        asm volatile("mov r0, %0"::"r" (x));
        asm volatile("swi #1");

}

/* calls software interrupt, then returns content of r0*/
char getChar() {
        asm volatile("swi #2");
        uint32_t holder = 0;
        asm volatile("mov %0, r0": "+r" (holder));
        return (char)holder;
}

/* put arguments on stack, call software interrupt */
uint8_t createThread(void (*func)(void *), const void * args, uint32_t args_size) {
        asm volatile("swi #3");
        return 1;
}

/* write exit code in register, call software interrupt */
void exit(uint32_t exitCode){
        asm volatile ("swi #42");
}

void sleep(uint32_t sleepTime) {

}