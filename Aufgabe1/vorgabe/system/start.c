#include "led.h"
#include "serial.h"
#include "kio.h"
#include "hwDefines.h"

void start_kernel(void)
{
  
	printGreeting();

  	char receivedChar;
 	while(1){
   		inputPresentation(receivedChar);
		}
 	}
}


void printGreeting(){
	kprintf("************************\n");		     
	kprintf("*gUt3N m0Rg3n <3 <3 UwU*\n");				     
	kprintf("************************\n\n\n\n");
}

void inputPresentation(char receivedChar){
	int hasReceived = uartReceiveChar(&receivedChar);
			if(hasReceived && receivedChar == 'i'){
				enableUartInterrupt();
			}
		    	if(hasReceived && receivedChar != 'i'){
				char charAsString[2] = "";
	      			charAsString[0] = receivedChar;
				kprintf("Char received. Character: %c | Integer: %i | Unsigned Integer: 					%u | Hex: %u | String: %s | Address: %p\n\n", receivedChar, 						receivedChar, receivedChar, receivedChar, charAsString, 					&receivedChar);
}
