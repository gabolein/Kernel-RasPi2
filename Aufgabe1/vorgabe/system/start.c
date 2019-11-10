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
        initTimer();
        enableUartInterrupt();
        specialMessage("Kernel loaded! (UwU)");
        while(1){
                char receivedChar;
                int hasReceived = uartReceiveChar(&receivedChar);
                if (hasReceived) {
                        switch(receivedChar){
                                case 'd': toggleDebugMode(); break;
                                //case 'e': enterSubProgramm(); break;
                                case 'c': register_checker(); break;
                                case 'a': causeDataAbort(); break;
                                case 'u': causeUndefinedInstruction(); break;
                                case 's': causeSWI(); break;
                                default: break;
                        }
                }
 	}
}
