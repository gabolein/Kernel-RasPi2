//
// Created by gabolein on 04.11.19.
//

#include "tests.h"
#include "kio.h"

void causeDataAbort() {
	int* a = (int*)0x1;
        kprintf("%x\n", *a);
}

void causeDataAbort0() { // causes Data abort
        int* a = (int*)0xaaaaaaaaa;
        kprintf("%x\n", *a);
}