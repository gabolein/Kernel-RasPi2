#include "serial.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"

#include <stdint.h>


void initUart() {
	//uartToggleTX(0);
	//uartToggleRX(0);

  	/* disable uart */
  	*uart_cr &= ~1;
  	while(*uart_fr & 0x4);
  	/* disable fifo */
  	*uart_lcrh &= ~(1 << 4);
	//*uart_cr = 0;
	//*uart_cr |= 1 << 9;
	//*uart_cr |= 1 << 8;
 	/* enable uart */
  	*uart_cr |= 1;
	//*uart_lcrh &= ~(1 << 4);

	//uartToggleTX(1);
	//uartToggleRX(1);
  
}

void kputChar(char c) {
 	while(uartTxFifoFull()){
 	}
 	*uart_dr = c; //Write dat shit
}

/* 
   Returns 1 if txFIFO is full 
*/
uint8_t uartTxFifoFull() {
  if(*uart_fr & 1 << 5){
    return 1;
  }
  return 0;
}


/*******************************************************/
/* Returns 1 if uart is currently transmitting a byte  */
/*******************************************************/
uint8_t uartBusy(){

  if(*uart_fr & 0x4){
    yellow_on();
    return 1;
  }
  return 0;

}



/**********************************************/
/* Returns 1 in case a char has been received */
/**********************************************/
uint8_t uartReceiveChar(char* c){
  //TODO Check for UART busy
  //Check for new data in receive buffer
	if(!uartRXFifoEmpty()){ 
		*c = *(const char*)uart_dr;
		
		return 1;
	}
	
 	return 0;
}


/***************************/
/* state = 1 -> TX enable  */
/* state = 0 -> TX disable */
/***************************/
void uartToggleTX(uint8_t state){
  if(state){
    *uart_cr |= 1 << 8; //Enable Transmit
    return;
  }
  *uart_cr ^= 1 << 8; //Disable Transmit
  return;
}

/***************************/
/* state = 1 -> RX enable  */
/* state = 0 -> RX disable */
/***************************/
void uartToggleRX(uint8_t state){
  if(state){
    *uart_cr |= 1 << 9; //Enable Receive
    return;
  }
  *uart_cr ^= 1 << 9; //Disable Receive
}

/***************************/
/* Returns 1 if Fifo empty */
/***************************/

uint8_t uartRXFifoEmpty(){
  if(*uart_fr & 1 << 4){
    //Fifo is empty
    return 1;
  }
  return 0;
}



char waitForReceive() {
  while(*uart_fr & (1 << 4));
  return (char)*uart_dr;
}


void enableUartInterrupt() {
	kprintf("uart_imsc: %x\n", *uart_imsc);
  	*uart_cr &= ~1;
	while(*uart_fr & 0x4);
	*uart_lcrh &= ~(1 << 4);
  	*enable_irq_2= 1 << 25;
	
  /* mask every bit except Receive interrupt */
	// clear all interrupts
	//*uart_icr |= 1 << 4;
	//*uart_cr &= ~1;
	//while(*uart_fr & 0x4);
	//*uart_lcrh &= ~(1 << 4);
	//*uart_imsc = 0xFFFFFFFF;
	*uart_imsc = 0;
	//kprintf("uart_imsc: %x\n", *uart_imsc);
  	//*uart_imsc &= ~(1 << 4);
	*uart_imsc |= 1 << 4;
	//*uart_cr = 0;
	//*uart_cr |= 1 << 9;
	//*uart_cr |= 1 << 8;
	//*uart_cr |= 1;
	*uart_cr |= 1;

	kprintf("uart_imsc: %x\n", *uart_imsc);
	kprintf("enable: %x\n", *enable_irq_2);
}
