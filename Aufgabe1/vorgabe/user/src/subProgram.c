#include "serial.h"
#include "handler.h"
#include "regcheck.h"

#define AMOUNT_CHARS 15
#define COMPUTATION_LEN 300000

void blockFunc() {
        volatile uint32_t baum = 0;
        for(int i = 0; i < COMPUTATION_LEN; i++){
                baum += 1;      /* "Berechnung" */
        }
}

void enterSubProgram() {
        subProgramMode = 1;
        char receivedChar;
        while(1){
                while(!(receivedChar = bufferGet()));

                for(int i = 0; i < AMOUNT_CHARS; i++){
                        if(checkerMode) {
                                register_checker();
                                checkerMode = 0;
                        }
                        kputChar(receivedChar);
                        blockFunc();
                }
        }
}
