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

void start_kernel(void)
{
        enableUartInterrupt();
        initMMU();
        mapIdleThread();
        specialMessage("Kernel loaded! (UwU)");
        initProcessArray();
        initTimer();
        initIdleThread();
        while(1);
}
