#include "scheduler.h"
#include "thread.h"
#include <stdint.h>

#define TIME_SLICE_INTERRUPTS   100
#define THREAD_ALIVE            3
#define THREAD_ARRAY_SIZE       32
#define IDLE_THREAD             32
#define NULL                    (void*)0

static uint16_t timerInterruptCount = 0;

/* returns index of next Thread that is up */
struct thcStruct* rrSchedule(struct thcStruct* currentThread, uint16_t threadDied) {
        timerInterruptCount++;
        if(currentThread == NULL) return &idleThread;
        if((threadDied)
           || (TIME_SLICE_INTERRUPTS <= timerInterruptCount)
           || currentThread == &idleThread
           || currentThread->status == WAITING) {
                timerInterruptCount = 0;
                if(currentThread == &idleThread){
                        currentThread = 0;
                        }
                uint8_t i = (currentThread + 1) % THREAD_ARRAY_SIZE;

                do {
                        if ((threadArray[i].status == READY || threadArray[i].status == RUNNING)) {
                                return i;
                        }
                        i = (i + 1) % THREAD_ARRAY_SIZE;
                } while (i != (currentThread + 1) % THREAD_ARRAY_SIZE);
                /* if we didnt find active Thread, activate idle thread */
                return IDLE_THREAD;
        }

        return currentThread;
}

void adjustSleptTime(){
	for(int i = 0; i < THREAD_ARRAY_SIZE; i++) {
            if(threadArray[i].status == WAITING && !threadArray[i].waitingForChar){
                threadArray[i].sleptTime++;
                if(threadArray[i].sleptTime >= threadArray[i].sleepingTime) {
                        threadArray[i].sleptTime = 0;
                        threadArray[i].status = READY;
                }
		}
	}
}
