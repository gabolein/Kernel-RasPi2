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


//kilian ist ein boot

void initUART() {
  
}

void printChar(char c) {
  //yellow_on(); //Debug
  //*UART_TX_RX_REG = c;
  UART_REG 
  while()
}

uint8_t txFifoFull() {
  
}
