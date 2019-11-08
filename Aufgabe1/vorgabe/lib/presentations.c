#include "presentations.h"
#include "kio.h"
#include "serial.h"
#include "registerDumpUtil.h"

void inputPresentation(char receivedChar) {
        char charAsString[2] = "";
        charAsString[0] = receivedChar;
        kprintf("Char received. Character: %c | Integer: %i | Unsigned Integer: %u "
                "| Hex: %x | String: %s | Address: %p\n\n",
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

        /* TODO Alles in einen Call packen */
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

        printMode(regDump->cpsr & 0x1F);
        kprintf("      (%x)\n", regDump->cpsr);

        {
                char spsrString[11];
                getPSRStrings(regDump->cpsr, spsrString);
                kprintf("SPSR: %s ", spsrString);
        }

        printMode(regDump->cpsr & 0x1F);
        kprintf("      (%x)\n\n", regDump->spsr);

        /* Aktuelle modusspezifische Register */
        kprintf(">>> Aktuelle modusspezifische Register <<<\n             LR         SP        SPSR\n");
        printModeRegs(USER_MODE,
                      regDump->userLr,
                      regDump->userSp,
                      0);
        printModeRegs(SUPERVISOR_MODE,
                      regDump->supervisorLr,
                      regDump->supervisorSp,
                      regDump->supervisorSpsr);
        printModeRegs(ABORT_MODE,
                      regDump->abortLr,
                      regDump->abortSp,
                      regDump->abortSpsr);
        printModeRegs(FAST_INTERRUPT_MODE,
                      regDump->fiqLr,
                      regDump->fiqSp,
                      regDump->fiqSpsr);
        printModeRegs(IRQ_MODE,
                      regDump->irqLr,
                      regDump->irqSp,
                      regDump->irqSpsr);
        printModeRegs(UNDEFINED_MODE,
                      regDump->undefinedLr,
                      regDump->undefinedSp,
                      regDump->undefinedSpsr);

        kprintf("\n\nSystem angehalten.\n");
}


/*
struct regDump* getRegDumpStruct(struct regDump* rd, ){

}
*/
