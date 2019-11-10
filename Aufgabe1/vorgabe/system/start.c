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
        /* initTimer(); */
        enableUartInterrupt();
        specialMessage("Kernel loaded! (UwU)");
        while(1);
}
