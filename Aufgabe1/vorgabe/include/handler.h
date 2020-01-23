#include <stdint.h>
#include "registerDumpUtil.h"

#ifndef HANDLER_H
#define HANDLER_H

extern void undefined_instruction(void*);
extern void prefetch_abort(void*);
extern void data_abort(void*);
extern void fiq();

#endif  /* HANDLER_H */
