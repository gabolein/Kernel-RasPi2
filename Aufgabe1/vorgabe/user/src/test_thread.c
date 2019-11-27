#include "../user/include/test_thread.h"
#include "thread.h"
#include "tests.h"

void threadCauseSWI() {
	kprintf("hereedww");
        endThread();
}

void threadCauseDataAbort() {
        causeDataAbort();
}

void threadCauseUndefinedInstruction() {
        causeUndefinedInstruction();
}
