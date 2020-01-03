#include "serial.h"
#include "thread.h"
#include "kio.h"

#define NULL 0



/* Erwartet den Char in R1 */
void putCharHandler() {
        uint32_t myChar = 0;
        asm volatile("mov %0, r1": "+r" (myChar));
	kprintf("content of r1: %i, %c\n", myChar, (char)myChar);
        kputChar((char)myChar);
}

/* returns char in r0 */
void getCharHandler() {
        char myChar = 0;
        if(uartReceiveChar(&myChar)){
                asm volatile("mov r0, %0":: "r" (myChar));
        }
}

/* Expects  funcpointer in r1, argCount in r2, args_size in r3*/
void newThreadHandler() {
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
        asm volatile("mov %0, r1": "+r" (func));
        asm volatile("mov %0, r2": "+r" (args));
        asm volatile("mov %0, r3": "+r" (args_size));
        createThread(func, args, args_size);
}

void exitHandler() {
        uint16_t currentThread = getRunningThread();
        killThread(currentThread);
}

/* Expects sleeptime in r1 */
void sleepHandler(){
        /* TODO */
}
