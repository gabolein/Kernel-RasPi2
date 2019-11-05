//
// Created by gabolein on 04.11.19.
//

#include "tests.h"

void causeDataAbort() {
	int a;
        asm volatile ("mov %0, #0x11":"=r"(a)); // write non aligned address
}
