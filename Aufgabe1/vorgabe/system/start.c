#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
#include "regcheck.h"
#include "thread.h"
#include <stdint.h>
#include "../user/include/threadUtil.h"
#include "../user/include/user_thread.h"

#define NULL 0

void start_kernel(void)
{
        enableUartInterrupt();
        specialMessage("Kernel loaded! (UwU)");
        initTimer();
        initThreadArray();
        createThread(&user_thread, NULL, 0); /* Init user thread */
        while(1);
}
