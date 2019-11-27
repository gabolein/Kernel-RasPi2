#include <kio.h>
#include "handler.h"
#include "regcheck.h"
#include "../user/include/subProgram.h"
#include "../user/include/user_thread.h"
#define AMOUNT_CHARS 15

void user_thread(void* arg) {
        kprintf("\n Yo ich bin der User_Thread\n");
        subProgramMode = 1;
        char receivedChar = (char)((*(uint32_t*)arg));

        for (int i = 0; i < AMOUNT_CHARS; i++) {
                if (checkerMode) {
                        //register_checker();
                        checkerMode = 0;
                }
                kprintf("%c", receivedChar);
                blockFunc();


        }
}
