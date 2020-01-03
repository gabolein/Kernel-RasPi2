#include "serial.h"
#include "thread.h"
#include "kio.h"
#include "handler.h"

#define NULL 0



/* Erwartet den Char in R1 */
void putCharHandler(struct regDump* rd) {
        uint32_t myChar = rd->r1;
        kputChar((char)myChar);
}

/* returns char in r1 */
void getCharHandler(struct regDump* rd) {
	char myChar = bufferGet();
        if(myChar){
		
		uint16_t currentThread = getRunningThread();
		threadArray[currentThread].context.r1 = myChar;
        }
	
}

/* Expects  funcpointer in r1, argCount in r2, args_size in r3*/
void newThreadHandler(struct regDump* rd) {
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
        asm volatile("mov %0, r1": "+r" (func));
        asm volatile("mov %0, r2": "+r" (args));
        asm volatile("mov %0, r3": "+r" (args_size));
        createThread(func, args, args_size);
}

void exitHandler(struct regDump* rd) {
        uint16_t currentThread = getRunningThread();
        killThread(currentThread);
}

/* Expects sleeptime in r1 */
void sleepHandler(struct regDump* rd){
        /* TODO */
}
