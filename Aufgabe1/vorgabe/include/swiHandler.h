#include "registerDumpUtil.h"
#ifndef SWIHANDLER_H
#define SWIHANDLER_H

void putCharHandler(struct regDump*);
void getCharHandler(struct regDump*);
void newThreadHandler(struct regDump*);
void exitHandler(struct regDump*);
void sleepHandler(struct regDump*);

#endif //SWIHANDLER_H
