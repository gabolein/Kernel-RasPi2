#include "registerDumpUtil.h"
#include <stdint.h>

#ifndef THREAD_H
#define THREAD_H
enum threadStatus {
        ALIVE       = 0,
        DEAD        = 1,
        ACTIVE      = 2,
        SLEEPING    = 3
};

struct thcStruct{
    struct commonRegs context;
    uint32_t spsr;
    enum threadStatus status;
    uint16_t threadID;
};

void initThreadArray();
void cpyStacktoTHC();
void createThread(void *(void *), const void*, int);
void startThread();

#endif //THREAD_H
