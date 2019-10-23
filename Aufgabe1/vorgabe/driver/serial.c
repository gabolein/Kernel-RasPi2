#include "serial.h"
#include "led.h"

#include <stdint.h>

//UART Register Declarations
#define UART_BASE_ADDR 0x7E201000
#define UART_DR        (*(uint32_t *) UART_BASE_ADDR)
#define UART_RSRECR    (*(uint32_t *) UART_BASE_ADDR + 0x4) 
#define UART_FR        (*(uint32_t *) UART_BASE_ADDR + 0x8)
#define UART_IBRD      (*(uint32_t *) UART_BASE_ADDR + 0x10)
#define UART_FBRD      (*(uint32_t *) UART_BASE_ADDR + 0x14)
#define UART_LCRH      (*(uint32_t *) UART_BASE_ADDR + 0x18)
#define UART_CR        (*(uint32_t *) UART_BASE_ADDR + 0x1C)
#define UART_IFLS      (*(uint32_t *) UART_BASE_ADDR + 0x20)
#define UART_IMSC      (*(uint32_t *) UART_BASE_ADDR + 0x24)
#define UART_RIS       (*(uint32_t *) UART_BASE_ADDR + 0x28)
#define UART_MIS       (*(uint32_t *) UART_BASE_ADDR + 0x2C)
#define UART_ICR       (*(uint32_t *) UART_BASE_ADDR + 0x30)
#define UART_DMACR     (*(uint32_t *) UART_BASE_ADDR + 0x34)
#define UART_ITCR      (*(uint32_t *) UART_BASE_ADDR + 0x38)
#define UART_ITIP      (*(uint32_t *) UART_BASE_ADDR + 0x3C)
#define UART_ITOP      (*(uint32_t *) UART_BASE_ADDR + 0x40)
#define UART_TDR       (*(uint32_t *) UART_BASE_ADDR + 0x44)

#define UART_DR_DATA   (*(uint32_t *) UART_DR + 0x3)


void initUART() {
  
}

void kputChar(char c) {
  while(!uartTxReady());
  UART_DR_DATA = c;
}

/* 
   Returns 1 if uart clear to send is set 
*/
uint8_t uartTxReady(){
  if(uartClearToSend() && !uartTxFifoFull()){
    return 1;
  }
  return 0;
}

/* 
   Returns 1 if uart clear to send is set 
*/
uint8_t uartClearToSend() {
  if(UART_FR & 0x1){
    return 1;
  }
  return 0;
}

/* 
   Returns 1 if txFIFO is full 
*/
uint8_t uartTxFifoFull() {
  if(UART_FR & 0x20){
    return 1;
  }
  return 0;
}

/* 
   Returns 1 if uart is currently transmitting a byte 
*/
uint8_t uartBusy(){
  if(UART_FR & 0x4){
    return 1;
  }
  return 0;
}
