#include "tests.h"
#include "kio.h"
#include <stdint.h>


#pragma GCC diagnostic ignored "-Wunused-variable"
void causeDataAbort() {
        volatile int b = *((uint32_t*)0x1);
}

void causeUndefinedInstruction() {
        asm volatile (".word 0xf7f0a000\n");
}

void causeSWI(){
        asm volatile ("SWI 0x42");
}
