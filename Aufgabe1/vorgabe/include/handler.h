#include <stdint.h>

#ifndef _BAR_H_
#define _BAR_H_

extern void undefined_instruction();
extern void software_interrupt();
extern void prefetch_abort();
extern void data_abort();
extern void not_used();
extern void irq();
extern void fiq();

#endif

