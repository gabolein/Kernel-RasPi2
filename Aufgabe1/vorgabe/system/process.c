#include "process.h"
#include "memory.h"
#include "kio.h"
#include "thread.h"	
#include <stdint.h>

#define AMOUNT_PROCESSES 8

struct processStruct processArray[AMOUNT_PROCESSES] = {0};

void initProcessArray() {
        kprintf("initProcessArray\n");
        for (uint16_t i = 0; i < AMOUNT_PROCESSES; i++){
                processArray[i].status = UNUSED;
                processArray[i].processID = i;
                processArray[i].lastThread = 6; /*last thread because we increment it later in the scheduler */
                initThreadArray(i);
        }
}

void createProcess(void(*func)(void *), const void * args, uint32_t args_size, uint16_t processID) {
        kprintf("in createProcess\n");
        int16_t newProcess = getFreeProcess();
        if (newProcess == -1) {
                kprintf("\nCan't create new thread.\n");
                return;
        }
        if(newProcess != 8){
                kprintf("Process %i created \n", newProcess);
                processArray[newProcess].status = USED;
        }
        cpyData(processID, newProcess);
        createThread(func, args, args_size, newProcess);
}

void cpyData(uint16_t processID, uint16_t newProcess) {
        map1on1();
        copyUserBlock(processID, newProcess);
}

int16_t getFreeProcess() {
	for (int i = 0; i < AMOUNT_PROCESSES; i++){
		if (processArray[i].status == UNUSED) {
			return i;
		}
	}
	kprintf("\n No free Process! \n");
	return -1;
}

/* assumes all the threads are dead*/
void endProcess(uint16_t processID) {
	processArray[processID].status = UNUSED;
	processArray[processID].lastThread = 6;

}

/* returns 1 if process still has threads that are alive, 0 if there are not, if no threads kill process*/
uint8_t checkProcessAlive(uint16_t currentProcess) {
    for(uint16_t i = 0; i < AMOUNT_THREADS; i++) {
    	if(processArray[currentProcess].threadArray[i].status != DEAD){
            return 1;    
        }
    }
    endProcess(currentProcess);
    return 0;
}
