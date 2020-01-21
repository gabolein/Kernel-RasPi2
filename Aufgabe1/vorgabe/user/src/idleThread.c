#include "idleThread.h"
#include "io.h"
#include "user_thread.h"
#include "swiInterface.h"
#include <stdint.h>

#define NULL 0

void goIdle() {
        printf("ich bin der idle thread \n");
        newProcess(&spawner, NULL, 0);
        printf("nach newProcess (goIdle) \n");
        while(1){
        	//printf("\ncyyyyka blyeeet\n");
        }
}
