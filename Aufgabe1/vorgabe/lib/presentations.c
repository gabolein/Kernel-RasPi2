//
// Created by gabolein on 30.10.19.
//

#include "presentations.h"
#include "kio.h"
#include "serial.h"

void inputPresentation(char receivedChar) {
        char charAsString[2] = "";
        charAsString[0] = receivedChar;
        kprintf("Char received. Character: %c | Integer: %i | Unsigned Integer: %u \
                | Hex: %x | String: %s | Address: %p\n\n",
                receivedChar, receivedChar, receivedChar,
                receivedChar, charAsString, &receivedChar);
}

void specialMessage(const char* msg){
        kprintf("\n\n---------------------------\n");
        kprintf("%s\n", msg);
        kprintf("---------------------------\n\n\n");    
}
