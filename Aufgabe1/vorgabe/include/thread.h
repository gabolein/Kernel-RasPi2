#include "registerDumpUtil.h"
#include <stdint.h>

#ifndef THREAD_H
#define THREAD_H
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
        uint32_t initialSp;
        volatile uint8_t hasRun;         /* 1 or 0 */
        volatile uint32_t sleepingTime;
        volatile uint32_t sleptTime;
        volatile uint8_t waitingForChar;
};

void initThreadArray();
void cpyStacktoTHC();
void createThread(void (*func)(void *), const void*, uint32_t);
uint16_t getRunningThread();
int16_t threadWaitingForChar();
uint16_t getDeadThread();
void endThread();
void killThread(uint16_t);
void saveContext(uint16_t, void*);
void changeContext(uint16_t, void*);
void fillStack(volatile struct commonRegs*, void*);
extern struct thcStruct threadArray[];

#endif //THREAD_H
