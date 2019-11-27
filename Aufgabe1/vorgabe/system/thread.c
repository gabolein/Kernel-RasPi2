#include "thread.h"
#include "registerDumpUtil.h"
#include <stdint.h>
#include "kio.h"

#define AMOUNT_THREADS          32
#define IDLE                    AMOUNT_THREADS
#define USER_SP                 0x20000
#define END_USER_STACK          0x10000
#define THREAD_STACK_SIZE       (USER_SP-END_USER_STACK)/(AMOUNT_THREADS+1)

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
        /*TODO*/

}

void createThread(void (*func)(void *), const void * args, uint32_t args_size) {
        uint16_t newThread = getDeadThread();
        threadArray[newThread].status = READY;
        threadArray[newThread].context.pc = (uint32_t)func;
        threadArray[newThread].context.lr = (uint32_t)&endThread;
        threadArray[newThread].spsr = 0x10; /* User Mode, sonst nichts gesetzt */
        //Stack mit Argumenten füllen
        if(args_size){
                volatile void* sp = (void*)threadArray[newThread].context.sp;
                sp -= args_size;
                /* Vielleicht funktioniert das nicht wenn der SP immer Wortaligned sein muss?
                 Es könnte ja gelten: arg_size & 4 != 0*/
                /* TODO */
                for(uint32_t offset = 0; offset < args_size; offset += 4){
                        *(uint32_t*)(sp + offset) = *(uint32_t*)(args + offset);
                }
                threadArray[newThread].context.r0 = (uint32_t)sp; /* SP als erstes Argument an Threadfunktion übergeben */
        }
}

void endThread() {
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
        threadArray[currentThread].status = DEAD;
        threadArray[currentThread].context.sp = threadArray[currentThread].initialSp;
        kprintf("\n\nThread %u angehalten.\n", threadArray[currentThread].threadID);
}

void saveContext(uint16_t currentThread, void* sp) {
        struct commonRegs* cr = (struct commonRegs*) sp;
        asm volatile ("mrs %0, SPSR": "=r" (threadArray[currentThread].spsr));
        threadArray[currentThread].context = *cr;
        threadArray[currentThread].status = READY;
}


void changeContext(uint16_t nextThread, void* sp){
	fillStack(&threadArray[nextThread].context, sp);
       	asm volatile("msr SPSR_cxsf, %0":: "r" (threadArray[nextThread].spsr)); /* vodoo scheisse kp */
       	threadArray[nextThread].status = RUNNING;
}

void fillStack(struct commonRegs* cr, void* sp){
        struct commonRegs* rd = (struct commonRegs*) sp;
        rd->r0 = cr->r0;
        rd->r1 = cr->r1;
        rd->r2 = cr->r2;
        rd->r3 = cr->r3;
        rd->r4 = cr->r4;
        rd->r5 = cr->r5;
        rd->r6 = cr->r6;
        rd->r7 = cr->r7;
        rd->r8 = cr->r8;
        rd->r9 = cr->r9;
        rd->r10 = cr->r10;
        rd->r11 = cr->r11;
        rd->r12 = cr->r12;
        rd->sp = cr->sp;
        rd->lr = cr->lr;
        rd->pc = cr->pc;
}
