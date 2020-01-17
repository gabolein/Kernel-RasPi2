#include <stdint.h>

#ifndef PROCESS
#define PROCESS

enum processStatus {
	USED,
	UNUSED
};

struct processStruct {
	enum processSatus status
};

void initProcessArray();
void createProcess(*(void *), const void *, uint32_t, uint16_t);
uint16_t getFreeProcess();
void cpyData(uint16_t, uint16_t);

#endif