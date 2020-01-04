#include "thread.h"
#include "registerDumpUtil.h"
#include <stdint.h>
#include "kio.h"
#include "../user/include/idleThread.h"
#include "../user/include/swiInterface.h"

#define AMOUNT_THREADS          32
#define IDLE                    AMOUNT_THREADS
#define USER_SP                 0x20000
#define END_USER_STACK          0x0FF9D
#define THREAD_STACK_SIZE       (USER_SP-END_USER_STACK)/(AMOUNT_THREADS+1)
#define NULL                    0

struct thcStruct threadArray[AMOUNT_THREADS+1];

void initThreadArray() {
        /* Init all threads as dead and give them a stackpointer */
        for (int i = 0; i < AMOUNT_THREADS+1; i++) {
                threadArray[i].status = DEAD;
                threadArray[i].context.sp = USER_SP+THREAD_STACK_SIZE*i;
                threadArray[i].initialSp = threadArray[i].context.sp;
                threadArray[i].threadID = i;
        }
        /* Init Idle Thread */
        threadArray[IDLE].status = RUNNING;
        threadArray[IDLE].context.lr = (uint32_t)&goIdle + 4;
        threadArray[IDLE].spsr = 0x10;
        threadArray[IDLE].cpsr = 0x13;
}

void createThread(void (*func)(void *), const void * args, uint32_t args_size) {
        uint16_t newThread = getDeadThread();
        threadArray[newThread].hasRun = 0;
        threadArray[newThread].status = READY;
        threadArray[newThread].context.lr = (uint32_t)func + 4; /* +4, da im trampoline 4 subtrahiert wird */
        threadArray[newThread].spsr = 0x10; /* User Mode, sonst nichts gesetzt */
        threadArray[newThread].cpsr = 0x13; /* SVC Mode */
        threadArray[newThread].userLR = (uint32_t)&exit;
        //Stack mit Argumenten füllen
        if(args_size){
                /* volatile void* sp = (void*)threadArray[newThread].context.sp; */
                volatile void* sp = (void*)threadArray[newThread].initialSp;
                sp -= args_size * 4;
                for(uint32_t offset = 0; offset < args_size; offset++){
                        *(uint32_t*)(sp + offset * 4) = *(uint32_t*)(args + offset * 4);
                }
                threadArray[newThread].context.r0 = (uint32_t)sp; /* SP als erstes Argument an Threadfunktion übergeben */
        }
}

void endThread() {
        kprintf("Calling swi to end thread");
        asm volatile ("SWI 21");
}

uint16_t getDeadThread(){
        for(uint16_t i = 0; i < AMOUNT_THREADS + 1; i++) {
                if(threadArray[i].status == DEAD){
                        return i;
                }
        }
        kprintf("\n Error determining dead Thread! \n");
        return 0;
}

uint16_t getRunningThread(){
        for(uint16_t i = 0; i < AMOUNT_THREADS + 1; i++) {
                if(threadArray[i].status == RUNNING){
                        return i;
                }
        }
        kprintf("\n Error determining running Thread! \n");
        return 0;
}

void killThread(uint16_t currentThread) {
        kprintf("\nICH BIN KILLTHREAD\n");
        threadArray[currentThread].status = DEAD;
        threadArray[currentThread].context.sp = threadArray[currentThread].initialSp;
        kprintf("\n\nThread %u angehalten.\n", threadArray[currentThread].threadID);
}

void saveContext(uint16_t currentThread, void* sp) { 
        struct commonRegs* cr = (struct commonRegs*) sp;
        asm volatile ("mrs %0, SPSR": "=r" (threadArray[currentThread].spsr));
	asm volatile ("mrs %0, CPSR": "=r" (threadArray[currentThread].cpsr));
	asm volatile ("mrs %0, lr_usr": "=r" (threadArray[currentThread].userLR));
        threadArray[currentThread].context = *cr;
        threadArray[currentThread].status = READY;
}

void changeContext(uint16_t nextThread, void* sp){
        kprintf("\nIch bin der Kontextwechsel\n");
        fillStack(&(threadArray[nextThread].context), sp);
       	asm volatile("msr SPSR_cxsf, %0":: "r" (threadArray[nextThread].spsr)); /* TODO Maybe include statusbits */
        asm volatile("msr lr_usr, %0":: "r" (threadArray[nextThread].userLR));
       	threadArray[nextThread].status = RUNNING;
        asm volatile("msr sp_usr, %0":: "r" ((threadArray[nextThread].context.sp) + 13 * 4));
       	kprintf("\n");
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
