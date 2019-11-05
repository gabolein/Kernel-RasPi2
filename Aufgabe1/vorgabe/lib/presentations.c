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

void registerDump(struct regDump* regDump){
        kprintf("#################################### \
#######################################\n");

        switch(regDump->exType) {
        case SOFTWARE_INTERRUPT:    kprintf("Software Interrupt"); break;
        case DATA_ABORT:            kprintf("Data Abort"); break;
        case UNDEFINED_INSTRUCTION: kprintf("Undefined Instruction"); break;
        case PREFETCH_ABORT:        kprintf("Prefetch Abort"); break;
        case IRQ:                   kprintf("Interrupt"); break;
        case FIQ:                   kprintf("Fast Interrupt"); break;
        default:                    kprintf("Undefined Interrupt"); break; /*Error*/
        }

        kprintf(" an Addresse %x\n", regDump->insAddress);
        
        if(regDump->accessAddress != NONE){
                if(regDump->accessStyle == READ){
                        kprintf("Zugriff: lesend auf Addresse %x\n", regDump->accessAddress);
                } else {
                        kprintf("Zugriff: schreibend auf Addresse %x\n", regDump->accessAddress);
                }
        }
        kprintf("Fehler: %s\n\n", regDump->faultName);
        kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n");
        kprintf("R0: %x  R8:  %x\n", regDump->r0, regDump->r8);
        kprintf("R1: %x  R9:  %x\n", regDump->r1, regDump->r9);
        kprintf("R2: %x  R10: %x\n", regDump->r2, regDump->r10);
        kprintf("R3: %x  R11: %x\n", regDump->r3, regDump->r11);
        kprintf("R4: %x  R12: %x\n", regDump->r4, regDump->r12);
        kprintf("R5: %x  SP:  %x\n", regDump->r5, regDump->sp);
        kprintf("R6: %x  LR:  %x\n", regDump->r6, regDump->lr);
        kprintf("R7: %x  PC:  %x\n", regDump->r7, regDump->pc);

        kprintf("\n\n>>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
}
