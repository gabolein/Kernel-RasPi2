//
// Created by gabolein on 04.11.19.
//

#include "tests.h"

void causeDataAbort() {
	int* a = (int*)0x1;
        kprintf("%x\n", *a);
}
