#include <stdint.h>

static inline void uartToggleTX(uint8_t);
static inline void uartToggleRX(uint8_t);
static inline uint8_t uartRXFifoEmpty();
static inline char waitForReceive();
static inline void disableUart();
static inline void enableUart();
