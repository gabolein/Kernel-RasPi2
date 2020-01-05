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
#include "scheduler.h"
#include "thread.h"
#include "swiHandler.h"
#include "../user/include/testThread.h"
#include "../user/include/user_thread.h"

#define BUFFER_SIZE 100
#define UART_IRQ_PENDING (1 << 25)
#define TIMER_IRQ_PENDING 1
#define USER 0x10
#define NULL 0
#define IDLE_THREAD 32
#define END_THREAD 3
#define SYSCALLS 5
#define SLEEP 4

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

/* TODO */
void (*swiHandlerArray[5])() = {
        putCharHandler,
        getCharHandler,
        newThreadHandler,
        exitHandler,
        sleepHandler
};

void toggleDebugMode(){
        debugMode = !debugMode;
        if (debugMode) {
                specialMessage("Debug Mode activated");
        } else {
                specialMessage("Debug Mode deactivated");
        }
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
                       kprintf("!");
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
		char myChar = 0;
		if(uartReceiveChar(&myChar)) {
			bufferInsert(myChar);
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
	if ((rd.spsr & 0x1F) == USER) {
		/* End Thread */
		uint16_t currentThread = getRunningThread();
		exitHandler(&rd, sp);
		uint16_t nextThread = rrSchedule(currentThread, 1);
                changeContext(nextThread, sp);                
		return;
	}
	/* Kill Kernel */
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
	while(1);
        return;
}
void software_interrupt(void* sp){
    	struct regDump rd; /* can actually take this from context */
        getRegDumpStruct(&rd, SOFTWARE_INTERRUPT, sp);
        if ((rd.spsr & 0x1F) == USER) {
                /* adjust lr*/
                struct commonRegs* stackStruct = (struct commonRegs*) sp;
                stackStruct->lr += 4;
                uint32_t swiID = 0;
                asm volatile("mov %0, r7": "=r" (swiID)); /* get syscall number */
                if (swiID < SYSCALLS) {
			uint16_t currentThread = getRunningThread();
                        swiHandlerArray[swiID](&rd, sp);
                        if (swiID == END_THREAD) {
                                uint16_t nextThread = rrSchedule(currentThread, 1);
                                changeContext(nextThread, sp);
                        }
			if (swiID == SLEEP) {
				saveContext(currentThread, sp);
                                uint16_t nextThread = rrSchedule(currentThread, 1);
                                changeContext(nextThread, sp);
                        }
                }
                return;
        }
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
        while(1);
        return;
}
void prefetch_abort(void* sp){
        struct regDump rd;
        getRegDumpStruct(&rd, PREFETCH_ABORT, sp);
        registerDump(&rd);
        //killOrDie(&rd);
        return;
}
void data_abort(void* sp){
        struct regDump rd;
        getRegDumpStruct(&rd, DATA_ABORT, sp);
	if ((rd.spsr & 0x1F) == USER) {
		/* End Thread */
		uint16_t currentThread = getRunningThread();
		exitHandler(&rd, sp);
		uint16_t nextThread = rrSchedule(currentThread, 1);
                changeContext(nextThread, sp);                
		return;
	}
        registerDump(&rd);
        kprintf("\n\nKernel dead.\n");
        while(1);
        return;
}

void irq(void* sp){
        if(debugMode == 1) {
                struct regDump rd;
                getRegDumpStruct(&rd, IRQ, sp);
                registerDump(&rd);
        }
        if(clockHandler()){
                uint16_t currentThread = getRunningThread();
                uint16_t nextThread = rrSchedule(currentThread, 0);
                if (currentThread != nextThread) {
                        saveContext(currentThread, sp);
                        changeContext(nextThread, sp);
                }
        }
        if(uartHandler()){
                uint16_t currentThread = getRunningThread();
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

void fiq(){
        //handle das business
        return;
}




void killOrDie(struct regDump* rd, void* sp) {
        if ((rd->spsr & 0x1F) == USER) {
                uint16_t currentThread = getRunningThread();
                killThread(currentThread);
                uint16_t nextThread = rrSchedule(currentThread, 1);
                changeContext(nextThread, sp);
        } else {
                kprintf("\n\nSystem angehalten.\n");
                while(1);
        }
}
