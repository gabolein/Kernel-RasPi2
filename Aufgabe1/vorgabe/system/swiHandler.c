#include "serial.h"
#include "thread.h"
#include "kio.h"
#include "handler.h"
#include "registerDumpUtil.h"
#include "presentations.h"

#define NULL 0


/* Expects char in r1 */
void putCharHandler(struct regDump* rd, void* sp) {
        uint32_t myChar = rd->r1;
        kputChar((char)myChar);
}

/* returns char in r1 */
void getCharHandler(struct regDump* rd, void* sp) {
        char myChar = bufferGet();
        if(myChar){
                struct commonRegs* stackStruct = (struct commonRegs*) sp;
                stackStruct->r1 = myChar;
                return;
        }
        threadArray[getRunningThread()].waitingForChar = 1;
        threadArray[getRunningThread()].status = WAITING;
}

/* Expects  funcpointer in r1, argCount in r2, args_size in r3*/
void newThreadHandler(struct regDump* rd, void* sp) {
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
        func = (void*)rd->r1;
        args = (void*)rd->r2;
        args_size = rd->r3;
        createThread(func, args, args_size);
}

void exitHandler(struct regDump* rd, void* sp) {
        uint16_t currentThread = getRunningThread();
	/* zero registers */
        threadArray[currentThread].context.r0 = 0;
        threadArray[currentThread].context.r1 = 0;
        threadArray[currentThread].context.r2 = 0;
        threadArray[currentThread].context.r3 = 0;
        threadArray[currentThread].context.r4 = 0;
        threadArray[currentThread].context.r5 = 0;
        threadArray[currentThread].status = DEAD;
        threadArray[currentThread].context.sp = threadArray[currentThread].initialSp;
        registerDump(rd);
        kprintf("\n\nThread %u was deaded as fuck.\n", threadArray[currentThread].threadID);
}

/* Expects sleeptime in r1 */
void sleepHandler(struct regDump* rd, void* sp){
        uint32_t sleeptime = rd->r1;
	uint16_t currentThread = getRunningThread();
	threadArray[currentThread].sleepingTime = sleeptime;
}
