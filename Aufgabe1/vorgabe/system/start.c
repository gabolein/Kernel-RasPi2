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
                if (hasReceived && receivedChar == 'i') {
                        enableUartInterrupt(); // see serial.c
                }
                if (hasReceived && receivedChar != 'i' && receivedChar != 'a'){
                        inputPresentation(receivedChar); // see presentations.c
		}
 	}
}
