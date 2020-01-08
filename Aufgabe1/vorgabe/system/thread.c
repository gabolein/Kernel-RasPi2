#include "thread.h"
#include "registerDumpUtil.h"
#include <stdint.h>
#include "kio.h"
#include "tests.h"
#include "memory.h"
#include "../user/include/idleThread.h"
#include "../user/include/swiInterface.h"
#include "../user/include/user_thread.h"

#define AMOUNT_THREADS          32
#define IDLE                    AMOUNT_THREADS
#define INITIAL_VIRTUAL_SP      0x100FFFF8
#define NULL                    0
#define INSTRUCTION 4

struct thcStruct threadArray[AMOUNT_THREADS+1];

void initThreadArray() {
        /* Init all threads as dead and give them a stackpointer */
        for (int i = 0; i < AMOUNT_THREADS+1; i++) {
                threadArray[i].status = DEAD;
                threadArray[i].initialSp = INITIAL_VIRTUAL_SP; /* Stackpointer berechnen */
                threadArray[i].context.sp = threadArray[i].initialSp;
                threadArray[i].threadID = i;
        }
}

void initIdleThread() {
        remapUserStack(IDLE);
        threadArray[IDLE].hasRun = 0;
        threadArray[IDLE].waitingForChar = 0;
        threadArray[IDLE].spsr = USER_MODE;
        threadArray[IDLE].status = RUNNING;
        threadArray[IDLE].context.lr = (uint32_t)&goIdle + INSTRUCTION;
        threadArray[IDLE].spsr = USER_MODE;
        threadArray[IDLE].cpsr = SUPERVISOR_MODE;
        threadArray[IDLE].userLR = (uint32_t)&exit;
        asm volatile("msr SPSR_cxsf, %0":: "r" (threadArray[IDLE].spsr));
        asm volatile("msr sp_usr, %0":: "r" (threadArray[IDLE].context.sp));
        asm volatile("msr lr_usr, %0":: "r" (threadArray[IDLE].userLR));
        asm volatile("mov lr, %0":: "r" (threadArray[IDLE].context.lr));
        asm volatile("subs pc, lr, #4"); /* Change Context to IDLE Thread */
}

void createThread(void (*func)(void *), const void * args, uint32_t args_size) {
        kprintf("Mein Character ist %c", *(char*)args);
        char argument = *(char*)args;
        int newThread = getDeadThread();
        if (newThread == -1) {
            kprintf("\nCan't create new thread.\n");
            return;
        }
        threadArray[newThread].hasRun = 0;
        threadArray[newThread].status = READY;
        threadArray[newThread].context.lr = (uint32_t)func + INSTRUCTION; /* +4, da im trampoline 4 subtrahiert wird */
        threadArray[newThread].spsr = USER_MODE; /* User Mode, sonst nichts gesetzt */
        threadArray[newThread].cpsr = SUPERVISOR_MODE; /* SVC Mode */
        threadArray[newThread].userLR = (uint32_t)&exit;
        threadArray[newThread].waitingForChar = 0;
        //Stack mit Argumenten füllen
        volatile void* sp = (void*)threadArray[newThread].initialSp;
        remapUserStack(newThread);
        if(args_size){
                kprintf("Mein Character ist %c", argument);
                sp -= args_size * INSTRUCTION;
                /* for(uint32_t offset = 0; offset < args_size; offset++){ */
                /*         *(uint32_t*)(sp + offset * INSTRUCTION) = *(uint32_t*)(args + offset * INSTRUCTION); //TODO */
                /* } */
                *(char*)sp = argument; /* TODO Maybe remove this later */
                threadArray[newThread].context.r0 = (uint32_t)sp; /* SP als erstes Argument an Threadfunktion übergeben THIS IS IMPORTANT!!! DO NOT TOUCH THIS EVER AGAIN!!1!!eins!!elf */
        }
        threadArray[newThread].context.sp = (uint32_t)sp;
}

int getDeadThread(){ /* FIX */
        for(uint16_t i = 0; i < AMOUNT_THREADS + 1; i++) {
                if(threadArray[i].status == DEAD){
                        return i;
                }
        }
        kprintf("\n Error determining dead Thread! \n");
        return -1;
}

int getRunningThread(){
        for(uint16_t i = 0; i < AMOUNT_THREADS + 1; i++) {
                if(threadArray[i].status == RUNNING){
                        return i;
                }
        }
        //kprintf("\n Error determining running Thread! \n");
        return -1;
}

int16_t threadWaitingForChar() {
        for(uint16_t i = 0; i < AMOUNT_THREADS + 1; i++) {
                if(threadArray[i].status == WAITING && threadArray[i].waitingForChar){
                        return i;
                }
        }
        return -1;
}

void killThread(uint16_t currentThread) {
        threadArray[currentThread].status = DEAD;
        threadArray[currentThread].context.sp = threadArray[currentThread].initialSp;
        kprintf("\n\nThread %u angehalten.\n", threadArray[currentThread].threadID);
}

void saveContext(uint16_t currentThread, void* sp) {
        struct commonRegs* cr = (struct commonRegs*) sp;
        asm volatile ("mrs %0, SPSR": "=r" (threadArray[currentThread].spsr));
        asm volatile ("mrs %0, CPSR": "=r" (threadArray[currentThread].cpsr));
        asm volatile ("mrs %0, lr_usr": "=r" (threadArray[currentThread].userLR));
        threadArray[currentThread].context = *cr; /* copy all common registers to thread context */
        asm volatile ("mrs %0, sp_usr": "=r" (threadArray[currentThread].context.sp));
        if(threadArray[currentThread].status == RUNNING) {
                threadArray[currentThread].status = READY;
        }
}

void changeContext(uint16_t nextThread, void* sp){
        remapUserStack(nextThread);
        fillStack(&(threadArray[nextThread].context), sp);
       	asm volatile("msr SPSR_cxsf, %0":: "r" (threadArray[nextThread].spsr));
        asm volatile("msr lr_usr, %0":: "r" (threadArray[nextThread].userLR));
        asm volatile("msr sp_usr, %0":: "r" (threadArray[nextThread].context.sp));
        threadArray[nextThread].status = RUNNING;
        /* kprintf("\n\n Changing to thread %i \n", nextThread); */
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
