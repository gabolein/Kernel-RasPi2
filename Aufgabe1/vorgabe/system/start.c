#include "presentations.h"
#include "serial.h"

void start_kernel(void)
{
	printGreeting(); // see presentations.c

  	char receivedChar;
 	while(1){
        int hasReceived = uartReceiveChar(&receivedChar);
        if (hasReceived && receivedChar == 'i') {
            enableUartInterrupt(); // see serial.c
        }
        if (hasReceived && receivedChar != 'i'){
   		    inputPresentation(receivedChar); // see presentations.c
		}
 	}
}


