#include <stdint.h>

static inline uint8_t uartRXFifoEmpty();
static inline char waitForReceive();
static inline void disableUart();
static inline void enableUart();
