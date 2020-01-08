#include "io.h"
#include <stdint.h>
#include "swiInterface.h"
#include "testThread.h"
#include "threadUtil.h"
#include "user_thread.h"


#define COMPUTATION_LEN 300000
#define AMOUNT_CHARS 3
#define CAPITAL_LETTER_LIM 97
#define SLEEP_TIME 5000
#define NULL 0

void user_thread(void* arg) {
        char receivedChar = *(char*)arg;
        /* if (*receivedChar_p == 's') { */
        /*         asm volatile ("swi #69"); */
        /* } else { */
        for (int i = 0; i < AMOUNT_CHARS; i++) {
                putChar(receivedChar);
                if (receivedChar < CAPITAL_LETTER_LIM) {
                	blockFunc();
                } else {
                	sleep(SLEEP_TIME);
                }
        }
}

void spawner() {
        uint32_t sp = 0;
        asm volatile ("mov %0, sp": "=r" (sp));
        printf("\nspawner active \n");
        printf("\nspawner sp: %x\n", sp);
        while(1) {

                volatile char c = getChar();
                if (c) {
                        uint32_t number = (uint32_t) c;
                        //newThread(&user_thread, &number, 1);
                        newThread(&entertainer, NULL, 0);
                }
        }
}

void entertainer() {
        while(1) {
                //sleep(3000);
                //char* string = "\n Hallo Freunde!\n";
                //newThread(&printer, string, 17);
        }
}

void printer(void* arg) {
        char* string = (char*)arg;
        printf(string);
}