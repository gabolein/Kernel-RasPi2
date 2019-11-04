//
// Created by gabolein on 04.11.19.
//

#import "tests.h"

void causeDataAbort(void) {
        asm("mov %2, #0x20123"::"r0"); // write non aligned address
        asm("ldr %2, [%2]"::"r0"); // load this address into register
        return 0;
}