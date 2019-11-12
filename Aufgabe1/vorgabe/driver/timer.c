#include "timer.h"
#include "hwDefines.h"
#include <stdint.h>

#define TIMER_INTERRUPT_ENABLE (1 << 5)
#define BASIC_IRQ_TIMER_ENABLE 1
#define BIG_COUNTER (1 << 1) /* 23 Bit */
#define TIMER_ENABLE (1 << 7)
#define TIMER_FREQUENCY 1 /* about 1 interrupt per second */
#define TIMER_INTERRUPT_STATUS 1

#define TIMER_PRE_DIV_VALUE 250000
#define TIMER_COUNTING_START 1823608 / TIMER_FREQUENCY
/* 250 MHz System timer Speed angenommen
   Prescaler: 256
   Counter Value: 9765623
   23-Bit Counter
 */

static volatile uint32_t* timer_load        = TIMER_LOAD;
static volatile uint32_t* timer_control     = TIMER_CONTROL;
static volatile uint32_t* timer_irq_clr_ack = TIMER_IRQ_CLR_ACK;
static volatile uint32_t* timer_masked_irq  = TIMER_MASKED_IRQ;
static volatile uint32_t* timer_pre_div     = TIMER_PRE_DIV;
static volatile uint32_t* enable_basic_irq  = ENABLE_BASIC_IRQ;


void initTimer() {
        *enable_basic_irq |= BASIC_IRQ_TIMER_ENABLE; /* Enable timer interrupt */
        *timer_load        = TIMER_COUNTING_START;
        *timer_control     = 1; /* Set Timer Prescaler */
        *timer_control    |= TIMER_INTERRUPT_ENABLE; /* Enable the interrupt at the timer as well */
        *timer_control    |= BIG_COUNTER;
        *timer_pre_div     = TIMER_PRE_DIV_VALUE; /* Durch 250mio teilen */
        *timer_control    |= TIMER_ENABLE;
}

void timerIrqClr() {
        /* A write clears the interrupt */
        *timer_irq_clr_ack = 1;
}

uint8_t timerCheckInterruptSet() {
        return (*timer_masked_irq & TIMER_INTERRUPT_STATUS);
}
