#include "process.h"
#include "memory.h"
#include "kio.h"
#include "thread.h"
#include <stdint.h>

#define AMOUNT_PROCESSES 8
#define ERROR            -1
#define LAST_THREAD      6

struct processStruct processArray[AMOUNT_PROCESSES] = {0};

void initProcessArray() {
        for (uint16_t i = 0; i < AMOUNT_PROCESSES; i++){
                processArray[i].status = UNUSED;
                processArray[i].processID = i;
                processArray[i].lastThread = LAST_THREAD; /*last thread because we increment it later in the scheduler */
                initThreadArray(i);
        }
}

void createProcess(void(*func)(void *), const void * args, uint32_t args_size, uint16_t processID) {
        int16_t newProcess = getFreeProcess();
        if (newProcess == ERROR) {
                kprintf("\nCan't create new process.\n");
                return;
        }
        kprintf("Process %i created \n", newProcess);
        processArray[newProcess].status = USED;
        /* Copy argument to Stack of Thread 0 (currently hardcoded to 1 byte) */
        char argumentCopy = *(char*)args;
        cpyData(processID, newProcess);
        nullHeap(newProcess);   /* has to be executed after cpyData because of map1on1 */
        createThread(func, &argumentCopy, args_size, newProcess);
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
        return ERROR;
}

/* assumes all the threads are dead*/
void endProcess(uint16_t processID) {
        processArray[processID].status = UNUSED;
        processArray[processID].lastThread = LAST_THREAD;
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
