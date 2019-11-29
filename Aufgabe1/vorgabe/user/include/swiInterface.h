#include <stdint.h>

#ifndef BSPRAK_SWIINTERFACE_H
#define BSPRAK_SWIINTERFACE_H

void putChar(char);
char getChar();
uint8_t createThread(void *(void *), const void *, uint32_t);
void exit(uint32_t);
void sleep(uint32_t);

#endif BSPRAK_SWIINTERFACE_H
