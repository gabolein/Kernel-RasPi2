#include "presentations.h"
#include "serial.h"
#include "tests.h"

void start_kernel(void)
{
	specialMessage("Kernel loaded! (UWU)");
        while(1){
                char receivedChar;
                int hasReceived = uartReceiveChar(&receivedChar);
                if (hasReceived && receivedChar == 'a') {
                        causeDataAbort();
                }
                else if (hasReceived && receivedChar == 'i') {
                        enableUartInterrupt(); // see serial.c
                }
                else if (hasReceived && receivedChar == 'p') {
                        causePrefetchAbort();
                }
                else if (hasReceived){
                        inputPresentation(receivedChar); // see presentations.c
		}
 	}
}
