#include "idleThread.h"
#include "io.h"
#include "user_thread.h"
#include "swiInterface.h"
#include <stdint.h>

#define NULL 0

void goIdle() {
		printf("\ngoIDle\n");
		newThread(&spawner, NULL, 0);
		volatile uint32_t sp = 0;
        while(1){
        	//asm volatile ("mov %0, sp": "=r" (sp));
        	//printf("\nsp of idle thread: %x\n", sp);
        	//printf("\nsp of idle thread: %x\n", sp);
        	//sleep(1000);
        	//printf("\nbin am chillen B)\n");
        }
}