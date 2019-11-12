#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
#include "regcheck.h"
#include <stdint.h>
#include "../user/include/subProgram.h"

void start_kernel(void)
{
        enableUartInterrupt();
        specialMessage("Kernel loaded! (UwU)");
        initTimer();

        kprintf("\n\nDezimal: %u   |   %x\n\n\n", 0x80000000, 0x80000000); /* TODO: HEX HAT NEN OVERFLOW */

        char receivedChar;
        while(1) {
                /* Anwendung */
                if((receivedChar = bufferGet())){
                        switch(receivedChar){
                        case 'd': toggleDebugMode();                                break;
                        case 'e': enterSubProgram();                                break;
                        case 'c': register_checker();                               break;
                        case 'a': causeDataAbort();                                 break;
                        case 'u': causeUndefinedInstruction();                      break;
                        case 's': causeSWI();                                       break;
                        default: kprintf("Received Character: %c\n", receivedChar); break;
                        }
                }
        }
}
