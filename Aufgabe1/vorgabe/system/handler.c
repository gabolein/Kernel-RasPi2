#include "handler.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"
#include "serial.h"
#include "presentations.h"
#include <stdint.h>
#include "handlerUtil.h"
#include "registerDumpUtil.h"
#include "timer.h"


/* Register Defs */
static volatile uint32_t* uart_icr  = UART_ICR;
static volatile uint32_t* irq_pending_2 = IRQ_PENDING_2;
static volatile uint32_t* irq_basic_pending = IRQ_BASIC_PENDING;

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

static inline void maskInterrupts() {
        asm volatile("cpsid if");
}

static inline void unmaskInterrupts() {
        asm volatile("cpsie if");
}

/* Handlers */
/* TODO: Pending bit clearen (oder wie auch immer das heißt)
   Prüfen um was für einen Interrupt es sich jeweils handelt
   Register vor dem Rücksprung wiederherstellen
*/
int clockHandler() {
        if (*irq_basic_pending & 0b1) {
                if (timerCheckInterruptSet()) {
                        kprintf("\n\n\nTimer Interrupt thrown!\n\n\n");
                }
                timerIrqClr();
                return 1;
        }
        else return 0;
}

int uartHandler() {
        if(*irq_pending_2 & (uint32_t)(1 << 25)){
                char receivedChar;
                uartReceiveChar(&receivedChar);
                kprintf("This character caused an interrupt: %c\n\n\n", receivedChar);
                *uart_icr = 0;
                return 1;
        }
        else return 0;
}
void undefined_instruction(void* sp){
        maskInterrupts();
        green_on();
        struct regDump rd;
        getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, sp);
        registerDump(&rd);
        unmaskInterrupts();
        //mov pc, lr
}
void software_interrupt(void* sp){
        maskInterrupts();
        red_on();
        struct regDump rd;
        getRegDumpStruct(&rd, SOFTWARE_INTERRUPT, sp);
        registerDump(&rd);
        unmaskInterrupts();
        //mov pc, lr
}
void prefetch_abort(void* sp){
        maskInterrupts();
        struct regDump rd;

        getRegDumpStruct(&rd, PREFETCH_ABORT, sp);
        registerDump(&rd);
        unmaskInterrupts();
        //mov pc, lr
}
void data_abort(void* sp){
        maskInterrupts();
        struct regDump rd;
        getRegDumpStruct(&rd, DATA_ABORT, sp);
        registerDump(&rd);
        unmaskInterrupts();
        //mov pc, lr
}

void irq(void* sp){
        maskInterrupts();
	if(debugMode == 1) {
		struct regDump rd;
                getRegDumpStruct(&rd, IRQ, sp);
                registerDump(&rd);
        }
        if (clockHandler()){
                while(1);
        }
        if (uartHandler()){
                while(1);
        }

        /* Clear all Interrupt state bits */

        kprintf("Thrown Interrupt couldn't be tracked\n");
        /* Abschmieren */
        while(1);
        //unmaskInterrupts();
        //mov pc, lr
}
void fiq(){
        maskInterrupts();
        //handle das business
        unmaskInterrupts();
        //mov pc, lr
}
