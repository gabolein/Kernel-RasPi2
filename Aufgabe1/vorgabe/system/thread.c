#include "thread.h"
#include "registerDumpUtil.h"
#include <stdint.h>

#define AMOUNT_THREADS          32
#define IDLE                    AMOUNT_THREADS
#define USER_SP                 0x20000
#define END_USER_STACK          0x10000
#define THREAD_STACK_SIZE       (USER_SP-END_USER_STACK)/(AMOUNT_THREADS+1)

static struct thcStruct threadArray[AMOUNT_THREADS+1];

void initThreadArray() {
        /* Init all threads as dead and give them a stackpointer */
        for (int i = 0; i < AMOUNT_THREADS+1; i++) {
                threadArray[i].status = DEAD;
                threadArray[i].context.sp = USER_SP+THREAD_STACK_SIZE*i;
                threadArray[i].threadID = i;
        }
        /* Init Idle Thread */
        threadArray[IDLE].status = ALIVE;
        /*TODO*/
}
