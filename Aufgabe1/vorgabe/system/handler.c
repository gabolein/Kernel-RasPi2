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
	char receivedChar;
  	/* Check what kind of interrupt is pending */
  	if(*irq_pending_2 & (uint32_t)(1 << 25)){ /* Check for UART Interrupt pending */
  	    /* UART interrupt going on! */
		uartReceiveChar(&receivedChar);
		kprintf("This character caused an interrupt: %c\n\n\n", receivedChar);
  	}
  
	*uart_icr = 0; //Clear all Interrupt state bits
	kprintf("I am useless now :D. Plz end me\n");
	while(1); // abschmieren
	
}
void fiq(){
  
}
