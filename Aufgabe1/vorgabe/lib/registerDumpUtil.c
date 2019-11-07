#include <stdint.h>
#include "kio.h"
#include "registerDumpUtil.h"


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


void printMode(enum Modes mode){
        switch(mode){
        case USER_MODE:           kprintf("User");               break;
        case FAST_INTERRUPT_MODE: kprintf("FIQ");                break;
        case IRQ_MODE:            kprintf("IRQ");                break;
        case SUPERVISOR_MODE:     kprintf("Supervisor");         break;
        case MONITOR_MODE:        kprintf("Monitor");            break;
        case ABORT_MODE:          kprintf("Abort");              break;
        case HYPERVISOR_MODE:     kprintf("Hypervisor");         break;
        case UNDEFINED_MODE:      kprintf("Undefined");          break;
        case SYSTEM_MODE:         kprintf("System");             break;
        default:                  kprintf("Mode field invalid"); break;
        }
}

void printModeRegs(enum Modes mode, uint32_t lr, uint32_t sp, uint32_t spsr) {
        if(mode == USER_MODE || mode == SYSTEM_MODE){
                kprintf("User/System: %x %x\n", lr, sp);
        } else {
                printMode(mode);
                {
                char psrString[11];
                getPSRStrings(spsr, psrString);
                kprintf(":  %x %x %s ", lr, sp, psrString);
                }
                printMode(spsr & 0x1F); /* Print mode thats in SPSR of the current mode */
                kprintf("    (%x)\n", spsr);
        }
}

