#include "../user/include/test_thread.h"
#include "thread.h"
#include "tests.h"
#include "kio.h"

void threadCauseSWI() {
}

void threadCauseDataAbort() {
        causeDataAbort();
}

void threadCauseUndefinedInstruction() {
        causeUndefinedInstruction();
}
