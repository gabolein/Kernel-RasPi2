#include "handler.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"
#include "serial.h"
#include <stdint.h>

/* Register Defs */
static volatile uint32_t* uart_icr  = UART_ICR;

static volatile uint32_t* irq_pending_2 = IRQ_PENDING_2;
/* Register Defs End */


/* General Procedure:
   Get Contents of DFSR in C variable using inline assembly.

 */

uint32_t getDFSRReg(){
        uint32_t dfsr;
        asm volatile("mcr p15, 0, %0, c5, c0, 0" : "=r" (dfsr));
        return dfsr;
}


void undefined_instruction(){
        green_on();
}
void software_interrupt(){
        red_on();
}
void prefetch_abort(){

}
void data_abort(){
        yellow_on();

        kprintf("%x\n", getDFSRReg());
}
void not_used(){
  
}
void irq(){ 
        //yellow_on();
        /* Check for pending UART Interrupt */
        if(*irq_pending_2 & (uint32_t)(1 << 25)){
                char receivedChar;
                uartReceiveChar(&receivedChar);
                kprintf("This character caused an interrupt: %c\n\n\n", receivedChar);
        }
        /* Clear all Interrupt state bits */
        *uart_icr = 0;
        kprintf("I am useless now :D. Plz end me\n");
        /* Abschmieren */
        while(1);
}
void fiq(){
  
}
