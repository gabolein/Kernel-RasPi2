#include "thread.h"
#include "process.h"
#include "registerDumpUtil.h"
#include <stdint.h>
#include "kio.h"
#include "tests.h"
#include "memory.h"
#include "timer.h"

#define AMOUNT_THREADS          7
#define IDLE_PID                8
#define AMOUNT_PROCESSES        8
#define IDLE_THREAD_INITIAL_SP  0x4E0000
#define NULL                    (void*)0
#define INSTRUCTION             4
#define USER_THREAD_FIRST_SP    0x420000
#define USER_THREAD_STACK_LEN   0x20000
#define ERROR                   -1
#define WORD                    4
#define USER_DATA_BASE          0x300000
#define IDLE_DATA_BASE          0x1400000
#define IDLE_THREAD_ID          8

struct thcStruct idleThread = {0};

/* User function names */
extern void exit();
extern void goIdle();

void initThreadArray(uint16_t currentProcess) {
        /* Init all threads as dead and give them a stackpointer */
        for (int i = 0; i < AMOUNT_THREADS; i++) {
                processArray[currentProcess].threadArray[i].status     = DEAD;
                processArray[currentProcess].threadArray[i].initialSp  = USER_THREAD_FIRST_SP + i * USER_THREAD_STACK_LEN; /* Stackpointer berechnen */
                processArray[currentProcess].threadArray[i].context.sp = processArray[currentProcess].threadArray[i].initialSp;
                processArray[currentProcess].threadArray[i].context.r1 = i; /* Neu erstellte Threads finden in r1 ihre ID vor */
                processArray[currentProcess].threadArray[i].threadID   = i;
                processArray[currentProcess].threadArray[i].processID  = currentProcess; /* amount processes*/
        }
}

static void copyIdleDataBlock(){
        map1on1();
        uint32_t* sourceAddr = (uint32_t*)(USER_DATA_BASE);
        uint32_t* targetAddr = (uint32_t*)(IDLE_DATA_BASE);
        /* Iteriere über 1 MB und kopiere wortweise */
        for(uint32_t currAddr = 0x0; currAddr < 0x100000; currAddr += WORD){
                *targetAddr = *sourceAddr;
                sourceAddr += 1;
                targetAddr += 1;
        }
}

void initIdleThread() { /* needs rework */
        asm volatile("cpsid i"); /* Make sure we won't get timer interrupts */
        copyIdleDataBlock();
        remapAddressSpace(IDLE_PID);
        idleThread.processID      = IDLE_THREAD_ID; /* IDLE Thread hat immer PID 8 */
        idleThread.hasRun         = 0;
        idleThread.waitingForChar = 0;
        idleThread.spsr           = USER_MODE;
        idleThread.status         = RUNNING;
        idleThread.context.lr     = (uint32_t)&goIdle + INSTRUCTION;
        idleThread.context.sp     = IDLE_THREAD_INITIAL_SP;
        idleThread.spsr           = USER_MODE;
        idleThread.cpsr           = SUPERVISOR_MODE;
        idleThread.userLR         = (uint32_t)&exit;
        asm volatile("msr SPSR_cxsf, %0" :: "r" (idleThread.spsr));
        asm volatile("msr sp_usr, %0"    :: "r" (idleThread.context.sp));
        asm volatile("msr lr_usr, %0"    :: "r" (idleThread.userLR));
        asm volatile("mov lr, %0"        :: "r" (idleThread.context.lr));
        asm volatile("subs pc, lr, #4"); /* Change Context to IDLE Thread */
}

void createThread(void (*func)(void *), const void * args, uint32_t args_size, uint16_t processID) {
        char argument = *(char*)args;
        int newThread = getDeadThread(processID);
        if (newThread == ERROR) {
                kprintf("\nCan't create new thread.\n");
                return;
        }
        processArray[processID].threadArray[newThread].context.r1     = newThread;
        processArray[processID].threadArray[newThread].hasRun         = 0;
        processArray[processID].threadArray[newThread].status         = READY;
        processArray[processID].threadArray[newThread].context.lr     = (uint32_t)func + INSTRUCTION; /* +4, da im trampoline 4
                                                                                                     subtrahiert wird */
        processArray[processID].threadArray[newThread].spsr           = USER_MODE;                          /* User Mode, sonst
                                                                                                               nichts gesetzt */
        processArray[processID].threadArray[newThread].cpsr           = SUPERVISOR_MODE;                    /* SVC Mode */
        processArray[processID].threadArray[newThread].userLR         = (uint32_t)&exit;
        processArray[processID].threadArray[newThread].waitingForChar = 0;

        /* Stack mit Argumenten füllen */
        volatile void* sp = (void*)processArray[processID].threadArray[newThread].initialSp;
        remapAddressSpace(processID);
        if(args_size){
                sp -= args_size * INSTRUCTION;
                *(char*)sp = argument;
                processArray[processID].threadArray[newThread].context.r0 = (uint32_t)sp; /* SP als erstes Argument an Threadfunktion übergeben
                                                                                             THIS IS IMPORTANT!!! DO NOT TOUCH THIS EVER AGAIN!!1!!eins!!elf */
        }
        processArray[processID].threadArray[newThread].context.sp = (uint32_t)sp;
}

int getDeadThread(uint16_t processID){
        for(uint16_t i = 0; i < AMOUNT_THREADS; i++) {
                if(processArray[processID].threadArray[i].status == DEAD){
                        return i;
                }
        }
        return -1;
}

struct thcStruct* getRunningThread(){ /* run over all processes */
        for(uint16_t i = 0; i < AMOUNT_PROCESSES; i++){
                for(uint16_t j = 0; j < AMOUNT_THREADS; j++) {
                        if(processArray[i].threadArray[j].status == RUNNING) {
                                return &(processArray[i].threadArray[j]);
                        }
                }
        }
        if(idleThread.status == RUNNING) {
                return &idleThread;
        }
        kprintf("\n Error determining running Thread! \n");
        return NULL;
}

struct thcStruct* threadWaitingForChar() {
        for(uint16_t i = 0; i < AMOUNT_PROCESSES; i++) {
                for(uint16_t j = 0; i < AMOUNT_THREADS; i++) {
                        if((processArray[i].threadArray[j].status == WAITING)
                           && (processArray[i].threadArray[j].waitingForChar)) {
                                return &(processArray[i].threadArray[j]);
                        }
                }
        }
        return NULL;
}

void saveContext(struct thcStruct* currentThread, void* sp) {
        struct commonRegs* cr = (struct commonRegs*) sp;
        asm volatile ("mrs %0, SPSR"   : "=r" (currentThread->spsr));
        asm volatile ("mrs %0, CPSR"   : "=r" (currentThread->cpsr));
        asm volatile ("mrs %0, lr_usr" : "=r" (currentThread->userLR));
        currentThread->context = *cr; /* copy all common registers to thread context */
        asm volatile ("mrs %0, sp_usr" : "=r" (currentThread->context.sp));
        if(currentThread->status == RUNNING) {
                currentThread->status = READY;
        }
}

void changeContext(struct thcStruct* nextThread, void* sp){
        remapAddressSpace(nextThread->processID);
        fillStack(&(nextThread->context), sp);
       	asm volatile("msr SPSR_cxsf, %0" :: "r" (nextThread->spsr));
        asm volatile("msr lr_usr, %0"    :: "r" (nextThread->userLR));
        asm volatile("msr sp_usr, %0"    :: "r" (nextThread->context.sp));
        nextThread->status = RUNNING;
}

void fillStack(volatile struct commonRegs* context, void* sp){
        volatile struct commonRegs* stackStruct = (struct commonRegs*) sp;
        stackStruct->r0  = context->r0;
        stackStruct->r1  = context->r1;
        stackStruct->r2  = context->r2;
        stackStruct->r3  = context->r3;
        stackStruct->r4  = context->r4;
        stackStruct->r5  = context->r5;
        stackStruct->r6  = context->r6;
        stackStruct->r7  = context->r7;
        stackStruct->r8  = context->r8;
        stackStruct->r9  = context->r9;
        stackStruct->r10 = context->r10;
        stackStruct->r11 = context->r11;
        stackStruct->r12 = context->r12;
        stackStruct->sp  = context->sp;
        stackStruct->lr  = context->lr;
        stackStruct->pc  = context->pc;
}
