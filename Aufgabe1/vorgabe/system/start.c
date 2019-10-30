#include "led.h"
#include "serial.h"
#include "kio.h"
#include "hwDefines.h"

void start_kernel(void)
{
  	//initUart();

  	//enableUartInterrupt();
  
  	//kprintf("Hello World!\n");
	kprintf("****************************************\n");		     
	kprintf("gUt3N m0Rg3n <3 <3 UWU \n");				     
	kprintf("****************************************\n\n\n\n");
	//kprintf("pending: %x", *irq_pending_2);
	//	uint32_t bumms = 0xD0909090;
	//	kprintf("testbyte:%x", bumms); 
	  
	//while(1){}

  	char receivedChar;
 	while(1){
   		int hasReceived = uartReceiveChar(&receivedChar);
		if(hasReceived && receivedChar == 'i'){
			enableUartInterrupt();
		}
	    	if(hasReceived && receivedChar != 'i'){
				
			kprintf("Char received. Character: %c\n", receivedChar);
		}
 	}
}
/*
  while(1){
    if(uartReceiveChar(&receivedChar)){
      yellow_on();
      char char_as_string[2] = "";
      char_as_string[0] = receivedChar;
      kprintf("\nI see you typed a character. Nice character, you are a man of culture.\n"
              "That character is so nice, let's see it once more: %c.\n"
              "Impressive right?\n"
              "Well, what if you wanted to see the ascii value of it? Surely that can't be poss...\n"
              "%u\n"
              "%x\n"
              "BAAM\n"
              "That's the Ascii value in decimal and hex for you.\n"
              "What if we casted %s as String? We just did.\n"
              "Otherwise, it could also very useful to know which address we saved that character in.\n"
              "Well, here it is: %p.\n"
              "Here's a %% for you too.\n"
              "Have a good day, plz gib max points <33\n\n",
              receivedChar, receivedChar, receivedChar, char_as_string, &receivedChar);
      kputChar(receivedChar);
    }
  }
  */


