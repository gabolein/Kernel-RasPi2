#include <stdint.h>

#ifndef PROCESS
#define PROCESS

enum processStatus {
	USED,
	UNUSED
};

struct processStruct {
	enum processStatus status;
	uint16_t processID;
};

void initProcessArray();
void createProcess(void(*)(void *), const void *, uint32_t, uint16_t);
int16_t getFreeProcess();
void cpyData(uint16_t, uint16_t);
void killProcess();
uint8_t checkProcessAlive(uint16_t);
extern struct processStruct processArray[];

#endif