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
                        switch(receivedChar){
                        case 'a': causeDataAbort(); break;
                        case 'u': causeUndefinedInstruction(); break;
                        case 's': causeSWI(); break;
                        case 'i': enableUartInterrupt(); break;
                        case 'p': printRegisterDump; break;
                        default: inputPresentation(receivedChar); break;
                        }
                }
 	}
}
