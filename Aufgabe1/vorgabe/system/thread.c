#include "thread.h"
#include "registerDumpUtil.h"
#include <stdint.h>
#include "kio.h"

#define AMOUNT_THREADS          32
#define IDLE                    AMOUNT_THREADS
#define USER_SP                 0x20000
#define END_USER_STACK          0x10000
#define THREAD_STACK_SIZE       (USER_SP-END_USER_STACK)/(AMOUNT_THREADS+1)

static struct thcStruct threadArray[AMOUNT_THREADS+1];

void initThreadArray() {
        /* Init all threads as dead and give them a stackpointer */
        for (int i = 0; i < AMOUNT_THREADS+1; i++) {
                threadArray[i].status = DEAD;
                threadArray[i].context.sp = USER_SP+THREAD_STACK_SIZE*i;
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
        threadArray[newThread].spsr = 0x2; /* User Mode, sonst nichts gesetzt */
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
