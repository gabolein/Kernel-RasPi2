#include "process.h"
#include "kio.h"
#include "thread.h"	
#include <stdint.h>

#define AMOUNT_PROCESSES 8

struct processStruct processArray[AMOUNT_PROCESSES] = {0};

void initProcessArray() {
	for (int i = 0; i < AMOUNT_PROCESSES; i++){

	}
}

void createProcess(void(*func)(void *), const void * args, uint32_t args_size, uint16_t processID) {

	int16_t newProcess = getFreeProcess();
	if (newProcess == -1) {
		kprintf("\nCan't create new thread.\n");
        return;
	}
	cpyData(processID, newProcess);
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

void endProcess() {
	
}