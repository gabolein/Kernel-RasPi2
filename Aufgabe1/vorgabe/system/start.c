#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
#include "regcheck.h"

volatile int debugMode;
void start_kernel(void)
{
        enableUartInterrupt();
        specialMessage("Kernel loaded! (UwU)");
        initTimer();
        register_checker();
        while(1);
}
