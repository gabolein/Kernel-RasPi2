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
        printf("Ich bin ein Thread, mein Stackpointer ist %x\n", (uint32_t)arg);
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
        while(1) {
                volatile char c = getChar();
                if (c) {
                        uint32_t number = (uint32_t) c;
                        newThread(&user_thread, &number, 1);
                        //newThread(&entertainer, NULL, 0);
                }
        }
}

void entertainer() {
        while(1) {
                sleep(3000);
                char* string = "Hallo Freunde!\n";
                newThread(&printer, string, 15);
        }
}

void printer(void* arg) {
        printf("hello i am the printer this is my song");
        char* string = (char*)arg;
        printf(string);
        printf("I have sung my song. I shall die now");
}
