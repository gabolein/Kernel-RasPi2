#include "idleThread.h"
#include "io.h"
#include "user_thread.h"
#include "swiInterface.h"
#include <stdint.h>

#define NULL 0

void goIdle() {
        newProcess(&spawner, NULL, 0);
        while(1);
}
