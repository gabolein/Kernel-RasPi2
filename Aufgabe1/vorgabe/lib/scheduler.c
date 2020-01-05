#include "scheduler.h"
#include "thread.h"
#include <stdint.h>

#define TIME_SLICE_INTERRUPTS   1
#define THREAD_ALIVE            3
#define THREAD_ARRAY_SIZE       32
#define IDLE_THREAD             32
static uint16_t timerInterruptCount = 0;

/* returns index of next Thread that is up */
uint16_t rrSchedule(uint16_t currentThread, uint16_t threadDied) {
        timerInterruptCount++;
        if((threadDied)
           || (TIME_SLICE_INTERRUPTS <= timerInterruptCount)
           || currentThread == IDLE_THREAD) {
                timerInterruptCount = 0;
                if(currentThread == IDLE_THREAD){
                        currentThread = 0;
                }
                uint8_t i = (currentThread + 1) % THREAD_ARRAY_SIZE;
                /* return first Thread that is alive */

                do {
                        if ((threadArray[i].status == READY
                            || threadArray[i].status == RUNNING) && threadArray[i].sleepingTime == 0) {
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
		if(threadArray[i].sleepingTime){
			threadArray[i].sleptTime++;
			if(threadArray[i].sleptTime >= threadArray[i].sleepingTime) {
				threadArray[i].sleepingTime = threadArray[i].sleptTime = 0;
			}
		}
	}
}
