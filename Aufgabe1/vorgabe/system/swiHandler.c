#include "serial.h"
#include "thread.h"
#include "kio.h"
#include "handler.h"
#include "../user/include/user_thread.h"

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
	kprintf("Entering new THread Handler\n");
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
	func = (void*)rd->r1;
	args = (void*)rd->r2;
	args_size = rd->r3;
	kprintf("func0: %x\n", &user_thread);
	kprintf("func: %x\n", func);
	// TODO: FIX THIS PIECE OF GARBAGE
        createThread(&user_thread, args, args_size);
	kprintf("Exeting new THread Handler\n");
}

void exitHandler(struct regDump* rd) {
        uint16_t currentThread = getRunningThread();
        killThread(currentThread);
}

/* Expects sleeptime in r1 */
void sleepHandler(struct regDump* rd){
        /* TODO */
}
