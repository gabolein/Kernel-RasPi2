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
#include "regcheck.h"
#include "tests.h"

static volatile uint32_t ledStatus = 0;
/* Register Defs */
static volatile uint32_t* uart_icr  = UART_ICR;
static volatile uint32_t* irq_pending_2 = IRQ_PENDING_2;
static volatile uint32_t* irq_basic_pending = IRQ_BASIC_PENDING;

/* Global CharBuffer */
static char charBuffer[100] = "";
static uint32_t charBufferLength = 0;


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

void toggleDebugMode(){ /* TODO: DO WE KEEP IT HERE?? */
        debugMode = !debugMode;
        if (debugMode) {
                specialMessage("Debug Mode activated");
        } else {
                specialMessage("Debug Mode deactivated");
        }
}
/* Handlers */
/* TODO: Pending bit clearen (oder wie auch immer das heißt)
   Prüfen um was für einen Interrupt es sich jeweils handelt
   Register vor dem Rücksprung wiederherstellen
*/
int clockHandler() {
        if (*irq_basic_pending & 0b1) {
                if (timerCheckInterruptSet()) {
                         /* kprintf("\n\n\nTimer Interrupt thrown!\n\n\n"); */
                }
                timerIrqClr();
                return 1;
        }
        return 0;
}

uint8_t bufferInsert(char c){
        if(charBufferLength >= 100){
                return 1;       /* Buffer is full */
        }
        charBufferLength++;
        charBuffer[charBufferLength - 1] = c;
        return 0;
}

char bufferGet() {

        if(charBufferLength <= 0) {
                return 0;
        }

        char returnChar = charBuffer[0];

        for(uint32_t i = 1; i < charBufferLength; i++){
                charBuffer[i - 1] = charBuffer[i];
        }
        return returnChar;
}

int uartHandler() {
        if(*irq_pending_2 & (uint32_t)(1 << 25)){
                char receivedChar;
                int hasReceived = uartReceiveChar(&receivedChar);
                if (hasReceived) {
                        bufferInsert(receivedChar);
                }
                *uart_icr = 0;
                return 1;
        }
        return 0;
}
void undefined_instruction(void* sp){
        green_on();
        struct regDump rd;
        getRegDumpStruct(&rd, UNDEFINED_INSTRUCTION, sp);
        registerDump(&rd);
        return;
}
void software_interrupt(void* sp){
        red_on();
        struct regDump rd;
        getRegDumpStruct(&rd, SOFTWARE_INTERRUPT, sp);
        registerDump(&rd);
        return;
}
void prefetch_abort(void* sp){
        struct regDump rd;

        getRegDumpStruct(&rd, PREFETCH_ABORT, sp);
        registerDump(&rd);
        return;
}
void data_abort(void* sp){
        struct regDump rd;
        getRegDumpStruct(&rd, DATA_ABORT, sp);
        registerDump(&rd);
        return;
}

void irq(void* sp){
        if(debugMode == 1) {
                struct regDump rd;
                getRegDumpStruct(&rd, IRQ, sp);
                registerDump(&rd);
        }
        if (clockHandler()){
                if(ledStatus){
                        yellow_off();
                        ledStatus = 0;
                }else{
                        yellow_on();
                        ledStatus = 1;
                }
        }
        if (uartHandler()){

        }

        /* kprintf("\nSpurious Interrupt ¯\\_(ツ)_/¯ \n"); */

        /* Clear all Interrupt state bits */
        /* Restore regular register status */
        /* restore modified lr */
        return;
}
void fiq(){
        //handle das business
        return;
}
