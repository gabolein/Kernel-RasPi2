#include <stdint.h>
#include "registerDumpUtil.h"

#ifndef _BAR_H_
#define _BAR_H_

extern void undefined_instruction(void*);
extern void prefetch_abort(void*);
extern void data_abort(void*);
extern void fiq();
void killOrDie(struct regDump*, void*);

#endif

