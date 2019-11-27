#ifndef TEST_THREAD_H
#define TEST_THREAD_H

void threadCauseSWI(void*);
void threadCauseDataAbort(void*);
void threadCauseUndefinedInstruction(void*);

#endif //TEST_THREAD_H
