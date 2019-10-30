//
// Created by gabolein on 30.10.19.
//

#include "presentations.h"
#include "kio.h"
#include "serial.h"

void printGreeting(){
    kprintf("************************\n");
    kprintf("*gUt3N m0Rg3n <3 <3 UwU*\n");
    kprintf("************************\n\n\n\n");
}

void inputPresentation(char receivedChar) {
        char charAsString[2] = "";
        charAsString[0] = receivedChar;
        kprintf("Char received. Character: %c | Integer: %i | Unsigned Integer: %u "
                "| Hex: %u | String: %s | Address: %p\n\n",
                receivedChar, receivedChar, receivedChar,
                receivedChar, charAsString, &receivedChar);
}

void printInterruptMode(){
    kprintf("\n\n---------------------------\n");
    kprintf("Entering interrupt mode ...\n");
    kprintf("---------------------------\n\n");
}