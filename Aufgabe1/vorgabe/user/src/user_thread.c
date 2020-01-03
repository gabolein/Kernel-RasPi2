#include <kioUser.h>
#include <stdint.h>
#include "swiInterface.h"
#include "testThread.h"
#include "threadUtil.h"


#define COMPUTATION_LEN 300000
#define AMOUNT_CHARS 15
#define CAPITAL_LETTER_LIM 97

void user_thread(void* arg) {
        char receivedChar = (char)*((uint32_t*)arg);
        if (receivedChar == 's') {
                asm volatile ("swi #69");
        }
        for (int i = 0; i < AMOUNT_CHARS; i++) {
                printf("%c", receivedChar);
                if (receivedChar < CAPITAL_LETTER_LIM) {
                        blockFunc();
                } else {
                        sleep(COMPUTATION_LEN);
                }
        }
}

void spawner() {
        while(1) {
		//putChar('d');
		//printf("Hallo freunde! %c\n", 'c');
		//putChar('a');
                volatile char c = getChar();
		//putChar('d');
		//printf("Hallo %i \n", 13);
		putChar('b');
                //newThread(&user_thread, &c, 1);
        }
}
