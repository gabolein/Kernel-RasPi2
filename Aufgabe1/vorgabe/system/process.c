#include "process.h"
#include <stdint.h>

struct processStruct processArray[AMOUNT_PROCESSES];

void initProcessArray() {
	for (int i = 0; i < AMOUNT_PROCESSES; i++){

	}
}

void createProcess((*func)(void *), const void * args, uint32_t args_size, uint16_t processID) {

	uint16_t newProcess = getFreeProcess();
	if (newProcess == -1) {
		kprintf("\nCan't create new thread.\n");
        return;
	}
	cpyData(processID, newProcess);
	createThread(func, args, args_size, newProcess);
}

void cpyData(uint16_t processID, uint16_t newProcess) {

}

uint16_t getFreeProcess() {
	for (int i = 0; i < AMOUNT_PROCESSES; i++){
		if (processArray[i].status == UNUSED) {
			return i;
		} 
	}
	kprintf("\n No free Process! \n");
	return -1;
}