#include "threadUtil.h"
#include <stdint.h>

#define AMOUNT_CHARS 15
#define COMPUTATION_LEN 300000

void blockFunc() {
        volatile uint32_t baum = 0;
        for(int i = 0; i < COMPUTATION_LEN; i++){
                baum += 1;      /* "Berechnung" */
        }
}

char* itoa10(unsigned int value, char* c_buffer){
        static char presentation[] = "0123456789";
        int i = 8; // start from the back
        do { // need do while in case value = 0
                c_buffer[i] = presentation[value % 10];
                --i;
                value /= 10;
        } while (value > 0);
        return &c_buffer[i+1];
}

char* itoa16(unsigned int value, char* c_buffer){
        static char presentation[] = "0123456789abcdef";
        int i = 9; // start from the back
        while (value > 0) { // need do while in case value = 0
                c_buffer[i] = presentation[value % 16];
                --i;
                value /= 16;
        }
        return c_buffer;
}
