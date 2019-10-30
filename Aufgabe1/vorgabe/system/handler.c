#include "handler.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"
#include "serial.h"


void undefined_instruction(){
  
}
void software_interrupt(){
  
}
void prefetch_abort(){
  
}
void data_abort(){
  
}
void not_used(){
  
}
void irq(){ 
 	yellow_on();
        /* Check for pending UART Interrupt */
  	if(*irq_pending_2 & (uint32_t)(1 << 25)){ 
                char receivedChar;
                uartReceiveChar(&receivedChar);
		kprintf("This character caused an interrupt: %c\n\n\n", receivedChar);
  	}
        /* Clear all Interrupt state bits */
  	*uart_icr = 0; 
	kprintf("I am useless now :D. Plz end me\n");
        /* Abschmieren */
        while(1);
}
void fiq(){
  
}
