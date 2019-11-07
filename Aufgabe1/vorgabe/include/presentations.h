#include <stdint.h>
#include "registerDumpUtil.h"

#ifndef _PRESENTATIONS_H
#define _PRESENTATIONS_H

void inputPresentation(char receivedChar);
void specialMessage(const char*);
void printRegisterDump(struct regDump*);

#endif
