#include "registerDumpUtil.h"
#include <stdint.h>

#ifndef THREAD_H
#define THREAD_H
enum threadStatus {
        WAITING     = 0,
        DEAD        = 1,
        RUNNING     = 2,
        READY       = 3
};

struct thcStruct{
    struct commonRegs context;
    uint32_t spsr;
    enum threadStatus status;
    uint16_t threadID;
    uint32_t initialSp;
};

void initThreadArray();
void cpyStacktoTHC();
void createThread(void (*func)(void *), const void*, uint32_t);
uint16_t getRunningThread();
uint16_t getDeadThread();
void endThread();
void killThread(uint16_t);
void saveContext(uint16_t, void*);
void changeContext(uint16_t, void*);
void fillStack(struct commonRegs*, void*);
extern struct thcStruct threadArray[];

#endif //THREAD_H
