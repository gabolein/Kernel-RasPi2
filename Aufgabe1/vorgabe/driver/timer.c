#include "timer.h"
#include "hwDefines.h"
#include <stdint.h>

#define TIMER_PRE_DIV_VALUE 0xEE6B27F

/* 250 MHz System timer Speed angenommen
   Prescaler: 256
   Counter Value: 9765623
   23-Bit Counter
 */

static volatile uint32_t* timer_load        = TIMER_LOAD;
/* static volatile uint32_t* timer_value       = TIMER_VALUE; */
static volatile uint32_t* timer_control     = TIMER_CONTROL;
static volatile uint32_t* timer_irq_clr_ack = TIMER_IRQ_CLR_ACK;
static volatile uint32_t* timer_masked_irq  = TIMER_MASKED_IRQ;
static volatile uint32_t* timer_pre_div     = TIMER_PRE_DIV;
static volatile uint32_t* enable_basic_irq  = ENABLE_BASIC_IRQ;


void initTimer() {
        *enable_basic_irq |= 0b1; /* Enable timer interrupt */
        *timer_load        = 0b1;
        *timer_control     = 0;         /* Set Timer Prescaler */
        *timer_control    |= 1 << 5;   /* Timer Interrupt */
        *timer_control    |= 1 << 1;   /* 23 Bit Counter */
        *timer_pre_div     = TIMER_PRE_DIV_VALUE; /* Durch 250mio teilen */
        *timer_control    |= 1 << 7;   /* Enable dat shit */
}

void timerIrqClr() {
        *timer_irq_clr_ack = 1;
}

uint8_t timerCheckInterruptSet() {
        return (*timer_masked_irq & 0b1);
}
