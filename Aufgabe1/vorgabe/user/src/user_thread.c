#include <kioUser.h>
#include <stdint.h>
#include "swiInterface.h"
#include "testThread.h"
#include "threadUtil.h"


#define COMPUTATION_LEN 300000
#define AMOUNT_CHARS 15
#define CAPITAL_LETTER_LIM 97

void user_thread(void* arg) {
	putChar('l');
        char* receivedChar = arg; /* TODO: correct argument passing, received Char always '2'?? */
        if (*receivedChar == 's') {
                asm volatile ("swi #69");
        } else {
        	for (int i = 0; i < AMOUNT_CHARS; i++) {
                	putChar(*receivedChar);
                	if (*receivedChar < CAPITAL_LETTER_LIM) {
                        	blockFunc();
                	} else {
                        	sleep(COMPUTATION_LEN);
                	}
        	}
		putChar('e');
	}
}

void spawner() {
        while(1) {;
                volatile char c = getChar();
		
		//putChar(c);
		if (c) {
			const void* charPointer = &c;
			//char* charPointerInt = charPointer; 
                	newThread(&user_thread, charPointer, 1);
			//putChar((char)*((uint32_t*)((const void*)(&c))));
			putChar('\n');
			putChar('r');
			putChar('\n');
			
		}
        }
}
