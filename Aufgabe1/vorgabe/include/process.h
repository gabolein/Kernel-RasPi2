#include <stdint.h>

#ifndef PROCESS
#define PROCESS

enum processStatus {
	USED,
	UNUSED
};

struct processStruct {
	enum processStatus status;
};

void initProcessArray();
void createProcess(void(*)(void *), const void *, uint32_t, uint16_t);
int16_t getFreeProcess();
void cpyData(uint16_t, uint16_t);
void killProcess();
extern struct processStruct processArray[];

#endif