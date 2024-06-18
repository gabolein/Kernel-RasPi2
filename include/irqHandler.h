#ifndef IRQHANDLER_H
#define IRQHANDLER_H

#include <stdint.h>

void toggleDebugMode();
uint8_t bufferInsert(char);
char bufferGet();
int uartHandler();
int clockHandler();
uint8_t bufferInsert(char);
char bufferGet();
extern void irq(void*);
extern volatile int debugMode;
extern uint8_t subProgramMode;
extern volatile uint8_t checkerMode;

#endif