#include "scheduler.h"
#include "thread.h"
#include <stdint.h>

#define TIME_SLICE_INTERRUPTS   10
#define THREAD_DEAD             1
#define THREAD_ALIVE            0
#define THREAD_ARRAY_SIZE       32
#define IDLE_THREAD             32
static uint16_t timerInterruptCount = 0;

/* returns index of next Thread that is up */
uint16_t rrSchedule(uint16_t currentThread, uint16_t threadDied) {
        if ((threadDied == THREAD_DEAD)
            || (threadDied == THREAD_ALIVE && TIME_SLICE_INTERRUPTS >= timerInterruptCount++)
            || currentThread == IDLE_THREAD) {
                timerInterruptCount = 0;
                uint8_t i = currentThread + 1;
                /* return first Thread that is alive */
                do {
                        if (threadArray[i].status == THREAD_ALIVE) {
                                return i;
                        }
                        i = (i + 1) % THREAD_ARRAY_SIZE;
                } while (i != currentThread + 1);
                /* if we didnt find active Thread, activate idle thread */
                return IDLE_THREAD;
        } else {
                return currentThread;
        }
}