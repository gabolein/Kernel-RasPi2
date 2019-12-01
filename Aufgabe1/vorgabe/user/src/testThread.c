#include "testThread.h"
#include "kio.h"
#include <stdint.h>

void threadCauseSWI() {
}

#pragma GCC diagnostic ignored "-Wunused-variable"
void threadCauseDataAbort() {
        volatile int b = *((uint32_t*)0x1);
}

void threadCauseUndefinedInstruction() {
        asm volatile (".word 0xf7f0a000\n");
}
