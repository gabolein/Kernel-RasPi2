#include "registerDumpUtil.h"
#ifndef SWIHANDLER_H
#define SWIHANDLER_H

void putCharHandler(struct regDump*, void*);
void getCharHandler(struct regDump*, void*);
void newThreadHandler(struct regDump*, void*);
void exitHandler(struct regDump*, void*);
void sleepHandler(struct regDump*, void*);

#endif //SWIHANDLER_H
