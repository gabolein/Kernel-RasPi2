#ifndef SERIAL_STATIC_H
#define SERIAL_STATIC_H

#include <stdint.h>

static inline uint8_t uartRXFifoEmpty();
static inline char waitForReceive();
static inline void disableUart();
static inline void enableUart();

#endif  /* SERIAL_STATIC_H */
