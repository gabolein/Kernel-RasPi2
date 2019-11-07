#include <stdint.h>
#include "kio.h"
#include "registerDumpUtil.h"

/* Defines */
#define DFSR_RW (1 << 11)
#define STATUS_LEADING (1 << 10)

/* Static inline functions */

static inline uint32_t getDFSR(){
        uint32_t dfsr = 0;
        asm volatile("mrc p15, 0, %0, c5, c0, 0" : "=r" (dfsr));
        return dfsr;
}

static inline uint32_t getDFAR(){
        uint32_t dfar = 0;
        asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r" (dfar));
        return dfar;
}

static inline uint32_t getLR() {
        uint32_t lr = 0;
        /* asm volatile("mov %0, lr" : "=r" (lr)); */
        return lr;
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

void getRegs(struct regDump* rd){

}

struct regDump* getRegDumpStruct(struct regDump* rd, enum ExceptionType exType ){
        rd->exType = exType;

        if(exType == DATA_ABORT) {
                uint32_t dfsr = getDFSR();
                rd->accessStyle = dfsr & DFSR_RW;
                rd->faultName = ((dfsr & STATUS_LEADING) << 4) + (dfsr & 0xF);
                rd->accessAddress = getDFAR();

                rd->insAddress = getLR() - 8;
        } else if(exType == UNDEFINED_INSTRUCTION ||
                  exType == PREFETCH_ABORT ||
                  exType == SOFTWARE_INTERRUPT){
                rd->insAddress = getLR() - 4;
        } else {
                rd->insAddress = 0;
        }

        /* TODO Register holen */

}
