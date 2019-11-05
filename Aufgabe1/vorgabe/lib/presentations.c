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

char* getPSRStrings(uint32_t psrReg, char* string){
        string = "NZCV E IFT";

        /* CPSRStatus */
        for(int i = 0; i < 4; i++){
                if(psrReg & (1 << (31 - i))){
                        continue;
                } else {
                        string[i] = '_';
                }
        }
        /* CPSRMask */
        for(int i = 0; i < 3; i++){
                if(psrReg & (1 << (7 - i))){
                        continue;
                } else {
                        string[i+7] = '_';
                }
        }

        /* E-Bit */
        if(psrReg & (1 << 9)){
                string[5] = 'E';
        } else {
                string[5] = '_';
        }
        
        return string;
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

        {
        char cpsrString[11];
        getPSRStrings(regDump->cpsr, cpsrString);
        kprintf("CPSR: %s ", cpsrString);
        }
        // TODO Funktion draus machen
        switch((regDump->cpsr & 0x1F)){
        case USER_MODE: kprintf("User"); break;
        case FAST_INTERRUPT_MODE: kprintf("FIQ"); break;
        case IRQ_MODE: kprintf("IRQ"); break;
        case SUPERVISOR_MODE: kprintf("Supervisor"); break;
        case MONITOR_MODE: kprintf("Monitor"); break;
        case ABORT_MODE: kprintf("Abort"); break;
        case HYPERVISOR_MODE: kprintf("Hypervisor"); break;
        case UNDEFINED_MODE: kprintf("Undefined"); break;
        case SYSTEM_MODE: kprintf("System"); break;
        default: kprintf("Mode field invalid"); break;
        }

        kprintf("      (%x)\n", regDump->cpsr);

        {
        char spsrString[11];
        getPSRStrings(regDump->cpsr, spsrString);
        kprintf("SPSR: %s ", spsrString);
        }
        //TODO Same wie oben
        switch((regDump->cpsr & 0x1F)){
        case USER_MODE: kprintf("User"); break;
        case FAST_INTERRUPT_MODE: kprintf("FIQ"); break;
        case IRQ_MODE: kprintf("IRQ"); break;
        case SUPERVISOR_MODE: kprintf("Supervisor"); break;
        case MONITOR_MODE: kprintf("Monitor"); break;
        case ABORT_MODE: kprintf("Abort"); break;
        case HYPERVISOR_MODE: kprintf("Hypervisor"); break;
        case UNDEFINED_MODE: kprintf("Undefined"); break;
        case SYSTEM_MODE: kprintf("System"); break;
        default: kprintf("Mode field invalid"); break;
        }
        kprintf("      (%x)\n\n", regDump->spsr);
}

