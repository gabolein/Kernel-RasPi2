#include "serial.h"
#include "thread.h"
#include "kio.h"
#include "handler.h"
#include "registerDumpUtil.h"
#include "../user/include/user_thread.h"

#define NULL 0



/* Erwartet den Char in R1 */
void putCharHandler(struct regDump* rd, void* sp) {
        uint32_t myChar = rd->r1;
        kputChar((char)myChar);
}

/* returns char in r1 */
void getCharHandler(struct regDump* rd, void* sp) {
	char myChar = bufferGet();
        if(myChar){
		
		//uint16_t currentThread = getRunningThread();
		struct commonRegs* stackStruct = (struct commonRegs*) sp;
		stackStruct->r1 = myChar;
		//threadArray[currentThread].context.r1 = myChar;
        }
	
}

/* Expects  funcpointer in r1, argCount in r2, args_size in r3*/
void newThreadHandler(struct regDump* rd, void* sp) {
	kprintf("Entering new THread Handler\n");
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
	func = (void*)rd->r1;
	args = rd->r2;
	args_size = rd->r3;
	kprintf("func0: %x\n", &user_thread);
	kprintf("func: %x\n", func);
	char* myChar = args;
	kprintf("\n This is the argssize: %i\n", args_size);
	// TODO: FIX THIS PIECE OF GARBAGE
        createThread(&user_thread, args, args_size);
}

void exitHandler(struct regDump* rd, void* sp) {
        uint16_t currentThread = getRunningThread();
        threadArray[currentThread].status = DEAD;
        threadArray[currentThread].context.sp = threadArray[currentThread].initialSp;
	kprintf("Entering Exit handler\n");
        kprintf("\n\nThread %u angehalten.\n", threadArray[currentThread].threadID);
	/* TODO MAybe set everything to 0 */
}

/* Expects sleeptime in r1 */
void sleepHandler(struct regDump* rd, void* sp){
        /* TODO */
}
