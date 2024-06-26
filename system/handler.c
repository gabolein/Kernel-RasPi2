#include "handler.h"
#include "led.h"
#include "kio.h"
#include "presentations.h"
#include <stdint.h>
#include "registerDumpUtil.h"
#include "scheduler.h"
#include "thread.h"
#include "swiHandler.h"

#define NULL            (void*)0
#define THREAD_DIED     1

#define DEBUG

void undefined_instruction(void* sp){
        green_on();
        struct regDump rd;
        getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, sp);
        if ((rd.spsr & 0x1F) == USER_MODE) {
                /* End Thread */
                struct thcStruct* currentThread = getRunningThread();
                exitHandler(&rd);
                struct thcStruct* nextThread = rrSchedule(currentThread, THREAD_DIED);
                changeContext(nextThread, sp);
                #ifdef DEBUG
                registerDump(&rd);
                #endif
                return;
        }
        /* Kill Kernel */
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
        while(1);
        return;
}

void prefetch_abort(void* sp){
        struct regDump rd;
        getRegDumpStruct(&rd, PREFETCH_ABORT, sp);
        if ((rd.spsr & 0x1F) == USER_MODE) {
                /* End Thread */
                struct thcStruct* currentThread = getRunningThread();
                exitHandler(&rd);
                struct thcStruct* nextThread = rrSchedule(currentThread, THREAD_DIED);
                changeContext(nextThread, sp);
                #ifdef DEBUG
                registerDump(&rd);
                #endif
                return;
        }
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
        while(1);
        return;
}
void data_abort(void* sp){
        struct regDump rd;
        getRegDumpStruct(&rd, DATA_ABORT, sp);
        if ((rd.spsr & 0x1F) == USER_MODE) {
                /* End Thread */
                struct thcStruct* currentThread = getRunningThread();
                exitHandler(&rd);
                struct thcStruct* nextThread = rrSchedule(currentThread, THREAD_DIED);
                changeContext(nextThread, sp);
                #ifdef DEBUG
                registerDump(&rd);
                #endif
                return;
        }
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
        while(1);
        return;
}



void fiq(){
        //handle das business
        return;
}
