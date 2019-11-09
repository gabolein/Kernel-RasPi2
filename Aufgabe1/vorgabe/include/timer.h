#include <stdint.h>

void initTimer();
void timerIrqClr();

/* 1 if Interrupt Set */
uint8_t timerCheckInteruptSet();
