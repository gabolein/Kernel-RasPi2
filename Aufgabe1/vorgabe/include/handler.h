#include <stdint.h>

#ifndef _BAR_H_
#define _BAR_H_

extern void undefined_instruction(void*);
extern void software_interrupt(void*);
extern void prefetch_abort(void*);
extern void data_abort(void*);
extern void irq();
extern void fiq();
int clockHandler();
int uartHandler();

#endif

