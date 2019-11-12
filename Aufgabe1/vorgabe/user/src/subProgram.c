#include "serial.h"
#include "handler.h"
#include "regcheck.h"

void blockFunc() {
        volatile uint32_t baum = 0;
        for(int i = 0; i < 300000; i++){
                baum += 1;      /* "Berechnung" */
        }
}

void enterSubProgram() {
        subProgramMode = 1;
        char receivedChar;
        while(1){
                while(!(receivedChar = bufferGet()));

                for(int i = 0; i < 15; i++){
                        if(checkerMode) {
                                register_checker();
                                checkerMode = 0;
                        }
                        kputChar(receivedChar);
                        blockFunc();
                }
        }
}
