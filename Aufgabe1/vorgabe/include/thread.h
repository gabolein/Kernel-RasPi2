#ifndef THREAD_H
#define THREAD_H

#include "registerDumpUtil.h"
#include <stdint.h>

enum threadStatus {
        WAITING     = 0,// Wartet auf irgendwas
        DEAD        = 1,// Ist tot
        RUNNING     = 2,// Ist grad dran
        READY       = 3 // Soll rankommen
};

struct thcStruct{
        volatile struct commonRegs context;
        volatile uint32_t spsr;
        volatile uint32_t cpsr;
        volatile uint32_t userLR;
        enum threadStatus status;
        uint16_t threadID;
        uint16_t processID;
        uint32_t initialSp;
        volatile uint8_t hasRun;         /* 1 or 0 */
        volatile uint32_t sleepingTime;
        volatile uint32_t sleptTime;
        volatile uint8_t waitingForChar;
};

void initThreadArray(uint16_t);
void initIdleThread();
void createThread(void (*)(void *), const void*, uint32_t, uint16_t);
struct thcStruct* getRunningThread();
struct thcStruct* threadWaitingForChar();
int getDeadThread(uint16_t processID);
void saveContext(struct thcStruct*, void*);
void changeContext(struct thcStruct*, void*);
void fillStack(volatile struct commonRegs*, void*);
extern struct thcStruct idleThread;

#endif //THREAD_H
