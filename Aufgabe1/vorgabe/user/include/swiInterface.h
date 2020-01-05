#include <stdint.h>

#ifndef SWIINTERFACE_H
#define SWIINTERFACE_H

void putChar(char);
char getChar();
void newThread(void (*)(void *), void *, uint32_t);
void exit();
void sleep(uint32_t);

#endif //SWIINTERFACE_H
