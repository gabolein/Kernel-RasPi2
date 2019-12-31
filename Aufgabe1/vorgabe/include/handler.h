#include <stdint.h>
#include "registerDumpUtil.h"

#ifndef _BAR_H_
#define _BAR_H_

extern void undefined_instruction(void*);
extern void software_interrupt(void*);
extern void prefetch_abort(void*);
extern void data_abort(void*);
extern void irq(void*);
extern void fiq();
void toggleDebugMode();
int clockHandler();
int uartHandler();
extern volatile int debugMode;
extern uint8_t subProgramMode;
extern volatile uint8_t checkerMode;
uint8_t bufferInsert(char);
void killOrDie(struct regDump*, void*);

/* SWI Handlers */
void putCharHandler();
void getCharHandler();
void newThreadHandler();
void exitHandler();
void sleepHandler();

/* Returns 0 in case of an empty buffer */
char bufferGet();

#endif

