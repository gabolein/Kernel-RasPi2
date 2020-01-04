#include <kioUser.h>
#include <stdint.h>
#include "swiInterface.h"
#include "testThread.h"
#include "threadUtil.h"


#define COMPUTATION_LEN 300000
#define AMOUNT_CHARS 15
#define CAPITAL_LETTER_LIM 97

void user_thread(void* arg) {
        char receivedChar = *(char*)arg; /* TODO: correct argument passing, received Char always '2'?? */
        /* if (*receivedChar_p == 's') { */
        /*         asm volatile ("swi #69"); */
        /* } else { */
        for (int i = 0; i < AMOUNT_CHARS; i++) {
                putChar(receivedChar);
                putChar('|');
                /* if (*receivedChar < CAPITAL_LETTER_LIM) { */
                /*     	blockFunc(); */
                /* } else { */
                /*         //	sleep(COMPUTATION_LEN); */
                /* } */
                //blockFunc();
                /* } */
        }
}

void spawner() {
        printf("Release the kraken\n");
        while(1) {
                volatile char c = getChar();
                if (c) {
                        uint32_t number = (uint32_t) c;
                        newThread(&user_thread, &number, 1);
                }
        }
}
