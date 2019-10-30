#include "handler.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"


void undefined_instruction(){
  
}
void software_interrupt(){
  
}
void prefetch_abort(){
  
}
void data_abort(){
  
}
void not_used(){
  
}
void irq(){ 
  yellow_on();
  /* asm("mov %CPSR, %SPSR"); */

  /* Check what kind of interrupt is pending */
  if(*irq_pending_2 & (uint32_t)(1 << 25)){ /* Check for UART Interrupt pending */
    /* UART interrupt going on! */
    kprintf("UART Interrupt received!\n");
  }
  
  /* shit going on here */


  *uart_icr = 0; //Clear all Interrupt state bits
  
  asm("b _reset");
  asm("mov %PC, %r14");
	
}
void fiq(){
  
}
