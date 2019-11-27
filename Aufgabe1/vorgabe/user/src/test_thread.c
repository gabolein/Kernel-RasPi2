#include "test_thread.h"
#include "thread.h"
#include "tests.h"

void threadCauseSWI() {
        endThread();
}

void threadCauseDataAbort() {
        causeDataAbort();
}

void threadCauseUndefinedInstruction() {
        causeUndefinedInstruction();
}
