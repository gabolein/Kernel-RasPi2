//
// Created by gabolein on 04.11.19.
//

#include "tests.h"
#include "kio.h"
#include <stdint.h>


#pragma GCC diagnostic ignored "-Wunused-variable"
void causeDataAbort() {
        volatile int b = *((uint32_t*)0x1);
}

void causeDataAbort0() { // causes Data abort
        int* a = (int*)0xaaaaaaaaa;
        kprintf("%x\n", *a);
}

void causeUndefinedInstruction() {
        asm volatile (".word 0xf7f0a000\n");
}

void causeSWI(){
        asm volatile ("SWI 0x42"); // works :)
}
