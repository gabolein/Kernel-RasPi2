#include "serial.h"
#include "led.h"

#include <stdint.h>

//UART Register Declarations
#define UART_BASE_ADDR (0x7E201000 - 0x3F000000)
#define UART_DR        ((uint32_t *) UART_BASE_ADDR)
#define UART_RSRECR    ((uint32_t *) UART_BASE_ADDR + 0x4) 
#define UART_FR        ((uint32_t *) UART_BASE_ADDR + 0x18)
#define UART_IBRD      ((uint32_t *) UART_BASE_ADDR + 0x24)
#define UART_FBRD      ((uint32_t *) UART_BASE_ADDR + 0x28)
#define UART_LCRH      ((uint32_t *) UART_BASE_ADDR + 0x2c)
#define UART_CR        ((uint32_t *) UART_BASE_ADDR + 0x30)
#define UART_IFLS      ((uint32_t *) UART_BASE_ADDR + 0x34)
#define UART_IMSC      ((uint32_t *) UART_BASE_ADDR + 0x38)
#define UART_RIS       ((uint32_t *) UART_BASE_ADDR + 0x3c)
#define UART_MIS       ((uint32_t *) UART_BASE_ADDR + 0x40)
#define UART_ICR       ((uint32_t *) UART_BASE_ADDR + 0x44)
#define UART_DMACR     ((uint32_t *) UART_BASE_ADDR + 0x48)
#define UART_ITCR      ((uint32_t *) UART_BASE_ADDR + 0x80)
#define UART_ITIP      ((uint32_t *) UART_BASE_ADDR + 0x84)
#define UART_ITOP      ((uint32_t *) UART_BASE_ADDR + 0x88)
#define UART_TDR       ((uint32_t *) UART_BASE_ADDR + 0x8c)

#define UART_DR_DATA   ((char *) (UART_DR + 0x3))


void initUart() {
  //Enable UART
  //*UART_CR |= (uint32_t) 1 << 0;

  //Enable FIFO
  *UART_LCRH |= 1 << 4;
  if(*UART_LCRH & 1 << 4) {
    //yellow_on();
  }
  
}

void kputChar(char c) {
  if(*UART_IMSC & 1 << 5){
    //Transmit Interrupt is set
    //yellow_on();
  }
    
  while(uartTxFifoFull()){
    yellow_on();
  }
  yellow_off();
  //yellow_on();
  *UART_DR |= c;
}















/* 
   Returns 1 if tx is ready
*/
uint8_t uartTxReady(){
  //if(uartClearToSend() && !uartTxFifoFull()){
  //if(uartClearToSend() && !uartTxFifoFull()){
  if(!uartTxFifoFull()){
    return 1;
  }
  return 0;
}

/* 
   Returns 1 if uart clear to send is set 
*/
uint8_t uartClearToSend() {
  if(*UART_FR & 1){
    return 1;
  }
  //yellow_on();
  return 0;
}

/* 
   Returns 1 if txFIFO is full 
*/
uint8_t uartTxFifoFull() {
  if(*UART_FR & 1 << 5){
    //yellow_on();
    return 1;
  }
  return 0;
}

/* 
   Returns 1 if uart is currently transmitting a byte 
*/
uint8_t uartBusy(){

  if(*UART_FR & 0x4){
    return 1;
  }
  
  return 0;

}
