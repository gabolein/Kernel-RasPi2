#include "handler.h"
#include "led.h"
#include "hwDefines.h"
#include "kio.h"
#include "serial.h"
#include "presentations.h"
#include <stdint.h>
#include "registerDumpUtil.h"
#include "timer.h"
#include "regcheck.h"
#include "tests.h"

#define BUFFER_SIZE 100
#define UART_IRQ_PENDING (1 << 25)

/* Register Defs */
static volatile uint32_t* uart_icr  = UART_ICR;
static volatile uint32_t* irq_pending_2 = IRQ_PENDING_2;
static volatile uint32_t* irq_basic_pending = IRQ_BASIC_PENDING;

/* Global CharBuffer */
static char charBuffer[BUFFER_SIZE] = "";
static uint32_t charBufferLength = 0;

/* Flags */
volatile uint32_t ledStatus = 0;
volatile int debugMode = 0;
uint8_t subProgramMode = 0;
volatile uint8_t checkerMode = 0;

void toggleDebugMode(){
        debugMode = !debugMode;
        if (debugMode) {
                specialMessage("Debug Mode activated");
        } else {
                specialMessage("Debug Mode deactivated");
        }
}

int clockHandler() {
        if (*irq_basic_pending & 1) {
                if (timerCheckInterruptSet()) {
                        if(ledStatus){
                                yellow_off();
                                ledStatus = 0;
                        }else{
                                yellow_on();
                                ledStatus = 1;
                        }
                        if(subProgramMode){
                                kprintf("!\n");
                        }
                }
                timerIrqClr();
                return 1;
        }
        return 0;
}

uint8_t bufferInsert(char c){
        if(charBufferLength >= BUFFER_SIZE){
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
        charBufferLength--;
        return returnChar;
}

int uartHandler() {
        if(*irq_pending_2 & UART_IRQ_PENDING){
                char receivedChar;
                int hasReceived = uartReceiveChar(&receivedChar);
                if (hasReceived) {
                        if(subProgramMode && (receivedChar == 'c')){
                                checkerMode = 1;
                        }
                        bufferInsert(receivedChar);
                }
                *uart_icr = 0;
                return 1;
        }
        return 0;
}

/* Begin C Handlers */

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
        clockHandler();
        uartHandler();
        return;
}
void fiq(){
        //handle das business
        return;
}
