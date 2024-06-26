#include "scheduler.h"
#include "process.h"
#include "thread.h"
#include "kio.h"
#include <stdint.h>

#define TIME_SLICE_INTERRUPTS   100
#define THREAD_ALIVE            3
#define THREAD_ARRAY_SIZE       7
#define PROCESS_ARRAY_SIZE	    8
#define IDLE_THREAD             32
#define NULL                    (void*)0
#define LAST_PROCESS             7

static uint16_t timerInterruptCount = 0;

void adjustSleptTime(){
        for(uint16_t i = 0; i < PROCESS_ARRAY_SIZE; i++) {
                for(uint16_t j = 0; j < THREAD_ARRAY_SIZE; j++) {
                        if(processArray[i].threadArray[j]. status == WAITING && !processArray[i].threadArray[j].waitingForChar){
                                processArray[i].threadArray[j].sleptTime++;
                                if(processArray[i].threadArray[j].sleptTime >= processArray[i].threadArray[j].sleepingTime) {
                                        processArray[i].threadArray[j].sleptTime = 0;
                                        processArray[i].threadArray[j].status = READY;
                                }
                        }
                }
        }
}

struct thcStruct* rrSchedule(struct thcStruct* currentThread, uint16_t threadDied) {
        timerInterruptCount++;
        if(currentThread == NULL) {
                return &idleThread;
        }
        if((threadDied)
           || (TIME_SLICE_INTERRUPTS <= timerInterruptCount)
           || currentThread == &idleThread
           || currentThread->status == WAITING) {
                timerInterruptCount = 0;
                if(currentThread == &idleThread){
                        currentThread = &(processArray[LAST_PROCESS].threadArray[0]); /* if coming from idlethread, start looking for new Thread at 0 */
                }
                uint8_t i = (currentThread->processID + 1) % PROCESS_ARRAY_SIZE; /* iterate over processes by starting after the current one */
                do {
                        if ((processArray[i].status == USED)) {
                                uint8_t j = (processArray[i].lastThread + 1) % THREAD_ARRAY_SIZE; /* we remember which thread was the last  */
                                do {
                                        if (processArray[i].threadArray[j].status == READY || processArray[i].threadArray[j].status == RUNNING) {
                                                processArray[i].lastThread = j;
                                                return &(processArray[i].threadArray[j]);
                                        }
                                        j = (j + 1) % THREAD_ARRAY_SIZE;
                                } while (j != (processArray[i].lastThread + 1) % THREAD_ARRAY_SIZE);
                        }
                        i = (i + 1) % PROCESS_ARRAY_SIZE;
                } while (i != (currentThread->processID + 1) % PROCESS_ARRAY_SIZE); /* end if we reach where we started iterating */
                return &idleThread;
        }
        return currentThread;
}
