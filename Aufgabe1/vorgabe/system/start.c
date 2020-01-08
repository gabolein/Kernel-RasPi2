#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
#include "regcheck.h"
#include "thread.h"
#include "memory.h"
#include <stdint.h>
#include "../user/include/threadUtil.h"
#include "../user/include/idleThread.h"
#include "../user/include/user_thread.h"

extern void spawner();

#define NULL 0

void start_kernel(void)
{
        enableUartInterrupt();
        initMMU();
        specialMessage("Kernel loaded! (UwU)");
        initThreadArray();
        initTimer();
        specialMessage("Timer activated");
        initIdleThread();
        while(1);
}
