
#ifndef PROCESS
#define PROCESS

#include <stdint.h>
#include "thread.h"

#define AMOUNT_THREADS 7

enum processStatus {
        USED,
        UNUSED
};

struct processStruct {
        enum processStatus status;
        uint16_t processID;
        struct thcStruct threadArray[AMOUNT_THREADS];
        uint16_t lastThread;
};

void initProcessArray();
void createProcess(void(*)(void *), const void *, uint32_t, uint16_t);
int16_t getFreeProcess();
void cpyData(uint16_t, uint16_t);
void killProcess();
uint8_t checkProcessAlive(uint16_t);
extern struct processStruct processArray[];

#endif
