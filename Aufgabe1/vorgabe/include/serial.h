#include <stdint.h>

void initUart();
void kputChar(char);
uint8_t uartTxFifoFull();
uint8_t uartTxReady();
uint8_t uartClearToSend();
