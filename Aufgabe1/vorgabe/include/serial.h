#include <stdint.h>

void initUart();
void kputChar(char);
uint8_t uartTxFifoFull();
uint8_t uartBusy();
uint8_t uartReceiveChar(char*);
void enableUartInterrupt();



