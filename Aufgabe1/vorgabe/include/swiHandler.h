#ifndef SWIHANDLER_H
#define SWIHANDLER_H

#include "registerDumpUtil.h"

enum syscallType {
	PUT_CHAR,
	GET_CHAR,
	NEW_THREAD,
	EXIT,
	SLEEP,
	NEW_PROCESS
};

void putCharHandler(struct regDump*);
void getCharHandler(void*);
void newThreadHandler(struct regDump*);
void exitHandler();
void sleepHandler(struct regDump*);
extern void software_interrupt(void*);


#endif /* SWIHANDLER_H */
