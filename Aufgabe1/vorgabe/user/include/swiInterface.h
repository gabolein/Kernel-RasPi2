#include <stdint.h>

#ifndef SWIINTERFACE_H
#define SWIINTERFACE_H

void putChar(char);
char getChar();
uint8_t newThread(void (*)(void *), const void *, uint32_t);
void exit(uint32_t);
void sleep(uint32_t);

#endif //SWIINTERFACE_H
