#include "threadUtil.h"

#define AMOUNT_CHARS 15
#define COMPUTATION_LEN 300000

void blockFunc() {
        volatile uint32_t baum = 0;
        for(int i = 0; i < COMPUTATION_LEN; i++){
                baum += 1;      /* "Berechnung" */
        }
}

