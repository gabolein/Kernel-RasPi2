#include "test_thread.h"
#include "thread.h"
#include "tests.h"

void threadCauseSWI(void*) {
        endThread();
}

void threadCauseDataAbort(void*) {
        causeDataAbort();
}

void threadCauseUndefinedInstruction(void*) {
        causeUndefinedInstruction();
}
