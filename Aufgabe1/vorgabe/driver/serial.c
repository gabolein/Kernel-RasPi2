#include "serial.h"
#include "led.h"

#include <stdint.h>

//UART Register Declarations
#define UART_BASE_ADDR (0x7E201000 - 0x3F000000)
#define UART_DR        ((uint32_t *) UART_BASE_ADDR)
#define UART_RSRECR    ((uint32_t *) (UART_BASE_ADDR + 0x4)) 
#define UART_FR        ((uint32_t *) (UART_BASE_ADDR + 0x18))
#define UART_IBRD      ((uint32_t *) (UART_BASE_ADDR + 0x24))
#define UART_FBRD      ((uint32_t *) (UART_BASE_ADDR + 0x28))
#define UART_LCRH      ((uint32_t *) (UART_BASE_ADDR + 0x2c))
#define UART_CR        ((uint32_t *) (UART_BASE_ADDR + 0x30))
#define UART_IFLS      ((uint32_t *) (UART_BASE_ADDR + 0x34))
#define UART_IMSC      ((uint32_t *) (UART_BASE_ADDR + 0x38))
#define UART_RIS       ((uint32_t *) (UART_BASE_ADDR + 0x3c))
#define UART_MIS       ((uint32_t *) (UART_BASE_ADDR + 0x40))
#define UART_ICR       ((uint32_t *) (UART_BASE_ADDR + 0x44))
#define UART_DMACR     ((uint32_t *) (UART_BASE_ADDR + 0x48))
#define UART_ITCR      ((uint32_t *) (UART_BASE_ADDR + 0x80))
#define UART_ITIP      ((uint32_t *) (UART_BASE_ADDR + 0x84))
#define UART_ITOP      ((uint32_t *) (UART_BASE_ADDR + 0x88))
#define UART_TDR       ((uint32_t *) (UART_BASE_ADDR + 0x8c))


static volatile uint32_t* uart_fr = UART_FR;
static volatile uint32_t* uart_dr = UART_DR;
static volatile uint32_t* uart_cr = UART_CR;
static volatile uint32_t* uart_ris = UART_RIS;
static volatile uint32_t* uart_lcrh = UART_LCRH;


void initUart() {
  
  *uart_cr |= 1 << 9; //Enable Receive
  *uart_cr |= 1 << 8; //Enable Transmit
  
}

void kputChar(char c) {
  while(uartTxFifoFull()){
  }
  *uart_dr |= c; //Write dat shit
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
  //if(*uart_ris & 1 << 4) {
  if(!(~*uart_dr & 0xFF)){ //Wenn Data Register nur Nullen enthält
    //Receive interrupt ist gesetzt
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
  *uart_cr |= 0 << 8; //Disable Transmit
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
  *uart_cr |= 0 << 9; //Disable Receive
}

