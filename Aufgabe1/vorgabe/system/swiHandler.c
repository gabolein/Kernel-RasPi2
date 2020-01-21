#include "swiHandler.h"
#include "scheduler.h"
#include "serial.h"
#include "thread.h"
#include "process.h"
#include "kio.h"
#include "handler.h"
#include "registerDumpUtil.h"
#include "presentations.h"
#include "irqHandler.h"
#include "memory.h"


#define NULL 0
#define INSTRUCTION 4


/* Expects char in r1 */
void putCharHandler(struct regDump* rd) {
        uint32_t myChar = rd->r1;
        kputChar((char)myChar);
}

/* returns char in r1 */
void getCharHandler(void* sp) {
        char myChar = bufferGet();
        if(myChar){
                struct commonRegs* stackStruct = (struct commonRegs*) sp;
                stackStruct->r1 = myChar;
                return;
        }
        struct thcStruct* currentThread = getRunningThread();
        currentThread->waitingForChar = 1;
        currentThread->status = WAITING;
}

/* Expects  funcpointer in r1, argCount in r2, args_size in r3*/
void newThreadHandler(struct regDump* rd) {
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
        func = (void*)rd->r1;
        args = (void*)rd->r2;
        args_size = rd->r3;
        uint16_t currentProcess = getRunningThread()->processID;
        createThread(func, args, args_size, currentProcess);
}

void exitHandler(struct regDump* rd) {
        struct thcStruct* currentThread = getRunningThread();
	/* zero registers */
        currentThread->context.r0 = 0;
        currentThread->context.r1 = 0;
        currentThread->context.r2 = 0;
        currentThread->context.r3 = 0;
        currentThread->context.r4 = 0;
        currentThread->context.r5 = 0;
        currentThread->status = DEAD;
        currentThread->context.sp = currentThread->initialSp;
        registerDump(rd);
        kprintf("\n\nThread %u was deaded as fuck.\n", currentThread->threadID);
        checkProcessAlive(currentThread->processID);
}

/* Expects sleeptime in r1 */
void sleepHandler(struct regDump* rd){
        uint32_t sleeptime = rd->r1;
        struct thcStruct* currentThread = getRunningThread();
        currentThread->sleepingTime = sleeptime;
        currentThread->status = WAITING;
}

void newProcessHandler(struct regDump* rd) {
        kprintf("newProcessHandler\n");
        uint32_t args_size = 0;
        void* args = NULL;
        void (*func)(void *) = NULL;
        func = (void*)rd->r1;
        args = (void*)rd->r2;
        args_size = rd->r3;
        uint16_t currentProcess = getRunningThread()->processID;
        createProcess(func, args, args_size, currentProcess);
}

void software_interrupt(void* sp){
        struct regDump rd;
        getRegDumpStruct(&rd, SOFTWARE_INTERRUPT, sp);
        if ((rd.spsr & 0x1F) == USER_MODE) {
                /* adjust lr*/
                struct commonRegs* stackStruct = (struct commonRegs*) sp;
                stackStruct->lr += INSTRUCTION;
                uint32_t swiID = 0;
                asm volatile("mov %0, r7": "=r" (swiID)); /* get syscall number */
                struct thcStruct* currentThread = getRunningThread();
                kprintf("in swi handler, swiID = %i", swiID);
                switch(swiID) {
                        case PUT_CHAR:
                                putCharHandler(&rd);
                                break;
                        case GET_CHAR:
                                getCharHandler(sp);
                                if(currentThread->status == WAITING) {
                                        saveContext(currentThread, sp);
                                        struct thcStruct* nextThread = rrSchedule(currentThread, 0);
                                        changeContext(nextThread, sp);
                                }
                                break;
                        case NEW_THREAD:
                                newThreadHandler(&rd);
                                remapAddressSpace(currentThread->processID);
                                break;
                        case EXIT:
                                exitHandler(&rd);
                                struct thcStruct* nextThreadExit = rrSchedule(currentThread, 1);
                                changeContext(nextThreadExit, sp);
                                break;
                        case SLEEP:
                                sleepHandler(&rd);
                                saveContext(currentThread, sp);
                                struct thcStruct* nextThread = rrSchedule(currentThread, 0);
                                changeContext(nextThread, sp);
                                break;
                        case NEW_PROCESS:
                                newProcessHandler(&rd);
                                remapAddressSpace(currentThread->processID);
                                kprintf("Ich bin im SWI Handler NEW_PROCESS\n");
                                break;
                        default:
                                kprintf("\nUNKNOWN SYSCALL\n");
                                break;

                }
                return;
        }
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
        while(1);
        return;
}
