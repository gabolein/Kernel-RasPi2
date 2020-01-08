#include "thread.h"
#include "registerDumpUtil.h"
#include "presentations.h"
#include "kio.h"
#include "hwDefines.h"
#include "serial.h"
#include "timer.h"
#include "scheduler.h"
#include "led.h"


#define BUFFER_SIZE 100
#define UART_IRQ_PENDING (1 << 25)
#define TIMER_IRQ_PENDING 1
#define IDLE_THREAD 32

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

/* Functions */

/* Inserts received character into buffer or forwards it to waitingThread if possible */
uint8_t bufferInsert(char c){
        if(charBufferLength >= BUFFER_SIZE){
                return 1;       /* Buffer is full */
        }
        int16_t waitingThread = threadWaitingForChar();
        if(waitingThread != -1){
                /* Thread den Char geben */
                threadArray[waitingThread].context.r1 = c;
                threadArray[waitingThread].waitingForChar = 0;
                threadArray[waitingThread].status = READY;
                return 0;
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
		char myChar = 0;
		if(uartReceiveChar(&myChar)) {
			bufferInsert(myChar);
		}
                *uart_icr = 0;
                return 1;
        }
        return 0;
}

int clockHandler() {
        if (*irq_basic_pending & TIMER_IRQ_PENDING) {
                if (timerCheckInterruptSet()) {
                        adjustSleptTime();
                        if(ledStatus){
                                yellow_off();
                                ledStatus = 0;
                        }else{
                                yellow_on();
                                ledStatus = 1;
                        }
                }
                timerIrqClr();
                return 1;
        }
        return 0;
}

void irq(void* sp){
        if(debugMode == 1) {
                struct regDump rd;
                getRegDumpStruct(&rd, IRQ, sp);
                registerDump(&rd);
        }
        if(clockHandler()){
                int currentThread = getRunningThread();
                uint16_t nextThread = rrSchedule(currentThread, 0);
                /*if(currentThread == -1){
                        changeContext(nextThread, sp);
                } else {*/
                        if ((currentThread != nextThread)&& currentThread != -1) {
                                saveContext(currentThread, sp);
                                changeContext(nextThread, sp);
                        }
                //}
        }
        if(uartHandler()){
                int currentThread = getRunningThread();
                if (currentThread == IDLE_THREAD) {
                        uint16_t nextThread = rrSchedule(currentThread, 0);
                        if (currentThread != nextThread) {
                                saveContext(currentThread, sp);
                                changeContext(nextThread, sp);
                        }
                }
        }
        return;
}
