#include "process.h"
#include "kio.h"
#include "thread.h"	
#include <stdint.h>

#define AMOUNT_PROCESSES 8

struct processStruct processArray[AMOUNT_PROCESSES] = {0};

void initProcessArray() {
	for (uint16_t i = 0; i < AMOUNT_PROCESSES; i++){
		processArray[i].status = UNUSED;
		processArray[i].processID = i;
	}
}

void createProcess(void(*func)(void *), const void * args, uint32_t args_size, uint16_t processID) {

	int16_t newProcess = getFreeProcess();
	if (newProcess == -1) {
		kprintf("\nCan't create new thread.\n");
        return;
	}
	cpyData(processID, newProcess);
	processArray[processID].status = USED;
	createThread(func, args, args_size, newProcess);
}

void cpyData(uint16_t processID, uint16_t newProcess) {
	
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

void endProcess(uint16_t processID) {
	processArray[processID].status = UNUSED;
}

/* returns 1 if process still has threads that are alive, 0 if there are not, if no threads kill process*/
uint8_t checkProcessAlive(uint16_t processID) {
    for(uint16_t i = 0; i < AMOUNT_PROCESSES; i++) {
    	if(threadArray[i].processID == processID){
            return 1;    
        }
    }
    endProcess(processID);
    return 0;
}