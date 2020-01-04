#include "swiInterface.h"
#include <stdint.h>
#include "kioUser.h"
#include "threadUtil.h"
#include "user_thread.h"

/* put given character into r1, then call software interrupt 1*/
void putChar(char x) {
	volatile uint32_t charCode = x;
	if(charCode){
        	asm volatile("mov r1, %0"::"r" (charCode));
        	asm volatile("mov r7, #0");
        	asm volatile("swi #0");
            asm volatile("mov r1, #0");
	}
}

/* calls software interrupt, then returns content of r0*/
char getChar() {
        asm volatile("mov r7, #1");
        asm volatile("swi #1");
        uint32_t holder = 0;
        asm volatile("mov %0, r1": "+r" (holder));
        asm volatile("mov r1, #0");
        return (char)holder;
}

/* put arguments in r1-r3, call software interrupt */
void newThread(void (*func)(void *), void * args, uint32_t args_size) {
        char* charPointer = args;
        putChar(args_size);
        putChar(*charPointer);
        putChar('\n');
        asm volatile("mov r3, %0"::"r" (args_size));
        asm volatile("mov r2, %0"::"r" (args));
        asm volatile("mov r1, %0"::"r" (func));
        asm volatile("mov r7, #2");
        asm volatile("swi #2");
        asm volatile("mov r1, #0");
        asm volatile("mov r2, #0");
        asm volatile("mov r3, #0");
        printf("\n\NewThread läuft durch\n\n");
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
