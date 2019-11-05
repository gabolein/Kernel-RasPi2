#include "presentations.h"
#include "serial.h"
#include "tests.h"

void start_kernel(void)
{
	specialMessage("Kernel loaded! (UWU)");
        while(1){
                char receivedChar;
                int hasReceived = uartReceiveChar(&receivedChar);
                if (hasReceived) { // TODO: build as switchcase
                        if (receivedChar == 'a') {
                                causeDataAbort();
                        }
                        else if (receivedChar == 'u') {
                                causeUndefinedInstruction();
                        }
                        else if (receivedChar == 's') {
                                causeSWI();
                        }
                        else if (receivedChar == 'i') {
                                enableUartInterrupt(); // see serial.c
                        }
                        else {
                                inputPresentation(receivedChar); // see presentations.c
                        }
                }
 	}
}
