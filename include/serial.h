#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void initUart();
void kputChar(char);
uint8_t uartTxFifoFull();
uint8_t uartBusy();
uint8_t uartReceiveChar(char*);
void enableUartInterrupt();

#endif  /* SERIAL_H */
