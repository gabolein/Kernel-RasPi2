#include "serial.h"
#include "led.h"

#define UART_BASE_ADDR 0x7E201000

typedef struct UART_REG_t {
  uint32_t UART_DR;
  uint32_t UART_RSRECR;
  uint32_t UART_FR;
  uint32_t unused0;
  uint32_t UART_IBRD;
  uint32_t UART_FBRD;
  uint32_t UART_LCRH;
  uint32_t UART_CR;
  uint32_t UART_IFLS;
  uint32_t UART_IMSC;
  uint32_t UART_RIS;
  uint32_t UART_MIS;
  uint32_t UART_ICR;
  uint32_t UART_DMACR;
  uint32_t UART_ITCR;
  uint32_t UART_ITIP;
  uint32_t UART_ITOP;
  uint32_t UART_TDR;
} UART_REG_t;


static volatile UART_REG_t* UART_REG const = (*UART_REG_t)(UART_BASE_ADDR);
  
static volatile char* UART_TX_RX_REG = (char*)(UART_REG + 23);

void printChar(char c) {
  //yellow_on(); //Debug
  //*UART_TX_RX_REG = c;
  UART_REG 
  while()
}

int txFifoFull() {
  
}
