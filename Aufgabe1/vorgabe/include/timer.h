#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void initTimer();
void timerIrqClr();

/* 1 if Interrupt Set */
uint8_t timerCheckInterruptSet();

#endif  /* TIMER_H */
