#include <stdint.h>

#ifndef HWDEFINES
#define HWDEFINES


/* UART Register Declarations */
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



/* Interrupts */
#define INTERRUPT_BASE                  (0x7E00B000 - 0x3F000000)
#define IRQ_BASIC_PENDING ((uint32_t *) (INTERRUPT_BASE + 0x200))
#define IRQ_PENDING_1     ((uint32_t *) (INTERRUPT_BASE + 0x204))
#define IRQ_PENDING_2     ((uint32_t *) (INTERRUPT_BASE + 0x208))
#define FIQ_CONTROL       ((uint32_t *) (INTERRUPT_BASE + 0x20C))
#define ENABLE_IRQ_1      ((uint32_t *) (INTERRUPT_BASE + 0x210))
#define ENABLE_IRQ_2      ((uint32_t *) (INTERRUPT_BASE + 0x214))
#define ENABLE_BASIC_IRQ  ((uint32_t *) (INTERRUPT_BASE + 0x218))
#define DISABLE_IRQ_1     ((uint32_t *) (INTERRUPT_BASE + 0x21C))
#define DISABLE_IRQ_2     ((uint32_t *) (INTERRUPT_BASE + 0x220))
#define DISABLE_BASIC_IRQ ((uint32_t *) (INTERRUPT_BASE + 0x224))


#endif
