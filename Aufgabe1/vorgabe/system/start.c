#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
volatile int debugMode;
void start_kernel(void)
{
        //initTimer();
        specialMessage("Kernel loaded! (UwU)");
        while(1){
                char receivedChar;
                int hasReceived = uartReceiveChar(&receivedChar);
                if (hasReceived) {
                        switch(receivedChar){
                                case 'd': debugMode = 1; break;
                                case 'a': causeDataAbort(); break;
                                case 'u': causeUndefinedInstruction(); break;
                                case 's': causeSWI(); break;
                                case 'i': enableUartInterrupt(); break;
                                default: inputPresentation(receivedChar); break;
                        }
                }
 	}
}
