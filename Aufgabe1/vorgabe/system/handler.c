#include "handler.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"
#include "serial.h"
#include "presentations.h"
#include <stdint.h>
#include "handlerUtil.h"
#include "registerDumpUtil.h"

/* Register Defs */
static volatile uint32_t* uart_icr  = UART_ICR;
static volatile uint32_t* irq_pending_2 = IRQ_PENDING_2;

/* General Procedure:
   Get Contents of DFSR in C variable using inline assembly.
*/

/* Static inline functions */
static inline uint32_t getDFSRReg(){
        uint32_t dfsr = 0;
        asm volatile("mrc p15, 0, %0, c5, c0, 0" : "=r" (dfsr));
        return dfsr;
}

static inline uint32_t getDFARReg(){
        uint32_t dfar = 0;
        asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r" (dfar));
        return dfar;
}



/* Handlers */
void undefined_instruction(void* pc){
        green_on();
        struct regDump rd;
        registerDump(getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, pc));
        while(1);
}
void software_interrupt(void* pc){
        red_on();
        struct regDump rd;
        registerDump(getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, pc));
        while(1);
}
void prefetch_abort(void* pc){
        struct regDump rd;
        registerDump(getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, pc));
        while(1);
}
void data_abort(void* pc){
        struct regDump rd;
        registerDump(getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, pc));
        while(1);
}

void irq(){
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
