#include "serial.h"
#include "serialStatic.h"
#include "hwDefines.h"
#include "kio.h"
#include "presentations.h"

#include <stdint.h>

#define ENABLE_RX (1 << 9)
#define ENABLE_TX (1 << 8)
#define UART_BUSY (1 << 2)
#define TX_FIFO_FULL (1 << 5)
#define RX_FIFO_EMPTY (1 << 4)
#define FIFO_ON (1 << 4)
#define UART_INTERRUPT_ENABLE (1 << 25)
#define RX_INTERRUPT (1 << 4)
#define UART_ON 1

/* Register Defs */
static volatile uint32_t* uart_fr = UART_FR;
static volatile uint32_t* uart_dr = UART_DR;
static volatile uint32_t* uart_cr = UART_CR;
static volatile uint32_t* uart_lcrh = UART_LCRH;
static volatile uint32_t* uart_imsc = UART_IMSC;

static volatile uint32_t* enable_irq_2  = ENABLE_IRQ_2;
/* Register Defs End */

static inline void disableUart() {
        *uart_cr &= ~UART_ON;
}

static inline void enableUart() {
        *uart_cr |= UART_ON;
}

void initUart() {
        disableUart();
        while(*uart_fr & UART_BUSY);
        *uart_cr |= ENABLE_RX;
        *uart_cr |= ENABLE_TX;
        enableUart();
}

void kputChar(char c) {
        while(uartTxFifoFull());
        *uart_dr = c;
}

/* Returns 1 if txFIFO is full */
uint8_t uartTxFifoFull() {
        if(*uart_fr & TX_FIFO_FULL){
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


/* Returns 1 if Fifo empty */
static inline uint8_t uartRXFifoEmpty(){
        if(*uart_fr & RX_FIFO_EMPTY){
                return 1;
        }
        return 0;
}

static inline char waitForReceive() {
        while(*uart_fr & RX_FIFO_EMPTY);
        return (char)*uart_dr;
}

void enableUartInterrupt() {
        disableUart();
        /* Disable FIFO */
        *uart_lcrh &= ~(FIFO_ON);
        *enable_irq_2 = UART_INTERRUPT_ENABLE;

        /* Mask every bit except receive Interrupt */
        *uart_imsc = 0;
        *uart_imsc |= RX_INTERRUPT;
        enableUart();
        specialMessage("Interrupt Mode activated");
}
