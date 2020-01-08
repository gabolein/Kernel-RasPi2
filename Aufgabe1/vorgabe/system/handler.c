#include "handler.h"
#include "led.h"
#include "kio.h"
#include "presentations.h"
#include <stdint.h>
#include "registerDumpUtil.h"
#include "scheduler.h"
#include "thread.h"
#include "swiHandler.h"


#define NULL 0

void undefined_instruction(void* sp){
        green_on();
        struct regDump rd;
        getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, sp);
        if ((rd.spsr & 0x1F) == USER_MODE) {
                /* End Thread */
                uint16_t currentThread = getRunningThread();
                exitHandler(&rd);
                uint16_t nextThread = rrSchedule(currentThread, 1);
                changeContext(nextThread, sp);
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
                uint16_t currentThread = getRunningThread();
                exitHandler(&rd);
                uint16_t nextThread = rrSchedule(currentThread, 1);
                changeContext(nextThread, sp);
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
                uint16_t currentThread = getRunningThread();
                exitHandler(&rd);
                uint16_t nextThread = rrSchedule(currentThread, 1);
                changeContext(nextThread, sp);
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
