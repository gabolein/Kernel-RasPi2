#include "serial.h"
#include "serialStatic.h"
#include "hwDefines.h"
#include "kio.h"
#include "presentations.h"

#include <stdint.h>

/* Register Defs */
static volatile uint32_t* uart_fr = UART_FR;
static volatile uint32_t* uart_dr = UART_DR;
static volatile uint32_t* uart_cr = UART_CR;
static volatile uint32_t* uart_lcrh = UART_LCRH;
static volatile uint32_t* uart_imsc = UART_IMSC;

static volatile uint32_t* enable_irq_2  = ENABLE_IRQ_2;
/* Register Defs End */

static inline void disableUart() {
        *uart_cr &= ~1;
}

static inline void enableUart() {
        *uart_cr |= 1;
}

void initUart() {
        disableUart();
        /* Wait for FIFO to be emptied by the hardware */
        while(*uart_fr & 0x4);
        /* Enable RX and TX */
        *uart_cr |= 1 << 9;
        *uart_cr |= 1 << 8;
        enableUart();
}

void kputChar(char c) {
        while(uartTxFifoFull());
        *uart_dr = c;
}

/* Returns 1 if txFIFO is full */
uint8_t uartTxFifoFull() {
        if(*uart_fr & 1 << 5){
                return 1;
        }
        return 0;
}

/* Returns 1 if UART is Busy */
uint8_t uartBusy(){
        if(*uart_fr & 0x4){
                return 1;
        }
        return 0;
}

/* Returns 1 in case a char has been received */
uint8_t uartReceiveChar(char* c){
        if(!uartRXFifoEmpty()){
		*c = *(const char*)uart_dr;
		return 1;
	}
 	return 0;
}

/* state = 1 -> TX enable  */
/* state = 0 -> TX disable */
static inline void uartToggleTX(uint8_t state){
        if(state){
                *uart_cr |= 1 << 8;
                return;
        }
        *uart_cr ^= 1 << 8;
        return;
}

/* state = 1 -> RX enable  */
/* state = 0 -> RX disable */
static inline void uartToggleRX(uint8_t state){
        if(state){
                *uart_cr |= 1 << 9;
                return;
        }
        *uart_cr ^= 1 << 9;
}

/* Returns 1 if Fifo empty */
static inline uint8_t uartRXFifoEmpty(){
        if(*uart_fr & 1 << 4){
                return 1;
        }
        return 0;
}

static inline char waitForReceive() {
        while(*uart_fr & (1 << 4));
        return (char)*uart_dr;
}

void enableUartInterrupt() {
        disableUart();
        //while(*uart_fr & 0x4); //TODO might not be necessary
        /* Disable FIFO */
        *uart_lcrh &= ~(1 << 4);
        *enable_irq_2= 1 << 25;

        /* Mask every bit except receive Interrupt */
        *uart_imsc = 0;
        *uart_imsc |= 1 << 4;
        enableUart();
        specialMessage("Interrupt Mode activated");
}
