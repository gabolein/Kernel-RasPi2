#include "presentations.h"
#include "timer.h"
#include "process.h"
#include "thread.h"
#include "memory.h"
#include "serial.h"

void start_kernel(void)
{
        enableUartInterrupt();
        initMMU();
        specialMessage("Kernel loaded! (UwU)");
        initProcessArray();
        initTimer();
        initIdleThread();
        while(1);
}
