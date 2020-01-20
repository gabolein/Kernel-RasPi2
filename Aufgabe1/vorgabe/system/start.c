#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
#include "regcheck.h"
#include "process.h"
#include "thread.h"
#include "memory.h"
#include <stdint.h>

extern void spawner();

#define NULL 0

void start_kernel(void)
{
        enableUartInterrupt();
        initMMU();
        specialMessage("Kernel loaded! (UwU)");
        remapUserStack(1);
        initProcessArray();
        initThreadArray();
        initTimer();
        initIdleThread();
        while(1);
}
