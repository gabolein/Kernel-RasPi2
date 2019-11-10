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
        kprintf("####################################"
                "#######################################\n");

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
        if(regDump->exType == DATA_ABORT) {
                if (regDump->accessStyle == READ) {
                        kprintf("Zugriff: lesend auf Addresse %x\n", regDump->accessAddress);
                } else {
                        kprintf("Zugriff: schreibend auf Addresse %x\n", regDump->accessAddress);
                }


                kprintf("Fehler: ");
                switch (regDump->faultName) {
                        case ALIGNMENT_FAULT:
                                kprintf("Alignment fault");
                                break;
                        case DEBUG_EVENT:
                                kprintf("Debug event fault");
                                break;
                        case ACCESS_FLAG_FAULT_ON_SECTION:
                                kprintf("Access Flag fault on Section");
                                break;
                        case CACHE_MAINTENANCE_FAULT:
                                kprintf("Cache maintenance operation fault[b]");
                                break;
                        case TRANSLATION_FAULT_ON_SECTION:
                                kprintf("Translation fault on Section");
                                break;
                        case ACCESS_FLAG_FAULT_ON_PAGE:
                                kprintf("Access Flag fault on Page");
                                break;
                        case TRANSLATION_FAULT_ON_PAGE:
                                kprintf("Translation fault on Page");
                                break;
                        case PRECISE_EXTERNAL_ABORT:
                                kprintf("Precise External Abort");
                                break;
                        case DOMAIN_FAULT_ON_SECTION:
                                kprintf("Domain fault on Section");
                                break;
                        case DOMAIN_FAULT_ON_PAGE:
                                kprintf("Domain fault on Page");
                                break;
                        case EXTERNAL_ABORT_ON_TRANSLATION_FIRST:
                                kprintf("External abort on translation, first level");
                                break;
                        case PERMISSION_FAULT_ON_SECTION:
                                kprintf("Permission fault on Section");
                                break;
                        case EXTERNAL_ABORT_ON_TRANSLATION_SECOND:
                                kprintf("External abort on translation, second level");
                                break;
                        case PERMISSION_FAULT_ON_PAGE:
                                kprintf("Permission fault on Page");
                                break;
                        case IMPRECISE_EXTERNAL_ABORT:
                                kprintf("Imprecise External Abort");
                                break;
                }
        }
        kprintf("\n\n>>> Registerschnappschuss (aktueller Modus) <<<\n");
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
        kprintf(">>> Aktuelle modusspezifische Register <<<\n              LR          SP        SPSR\n");
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
