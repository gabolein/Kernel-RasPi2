#include <stdint.h>

void initUart();
void kputChar(char);
uint8_t uartTxFifoFull();
uint8_t uartBusy();
uint8_t uartReceiveChar(char*);
static inline void uartToggleTX(uint8_t);
static inline void uartToggleRX(uint8_t);
static inline uint8_t uartRXFifoEmpty();
static inline char waitForReceive();
void enableUartInterrupt();
static inline void disableUart();
static inline void enableUart();


