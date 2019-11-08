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

static inline uint32_t getCPSR() {
        uint32_t cpsr = 0;
        asm volatile ("mrs %0, CPSR": "=r" (cpsr) );
        return cpsr;
}

static inline uint32_t getSPSR() {
        uint32_t spsr = 0;
        asm volatile ("mrs %0, SPSR": "=r" (spsr));
        return spsr;
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
                kprintf("    (%x)\n", spsr); /* TODO: Problem, brauchen getPSR String */
        }
}

void getRegs(struct regDump* rd, void* sp){
        struct commonRegs* regs = sp;
        (uint32_t*) r2 = sp + 56;
        kprintf("!!!!!R2: %x!!!!!", *r2);

        *((struct commonRegs*)rd) = *regs;
        return;
}

void getSpecialRegs(struct regDump* rd) {
    asm volatile ("mrs %0, lr_usr" : "=r" (rd->userLr));
    asm volatile ("mrs %0, sp_usr" : "=r" (rd->userSp));
    asm volatile ("mrs %0, lr_svc" : "=r" (rd->supervisorLr));
    asm volatile ("mrs %0, sp_svc" : "=r" (rd->supervisorSp));
    asm volatile ("mrs %0, spsr_svc" : "=r" (rd->supervisorSpsr));
    asm volatile ("mrs %0, lr_abt" : "=r" (rd->abortLr));
    asm volatile ("mrs %0, sp_abt" : "=r" (rd->abortSp));
    asm volatile ("mrs %0, spsr_abt" : "=r" (rd->abortSpsr));
    asm volatile ("mrs %0, lr_fiq" : "=r" (rd->fiqLr));
    asm volatile ("mrs %0, sp_fiq" : "=r" (rd->fiqSp));
    asm volatile ("mrs %0, spsr_fiq" : "=r" (rd->fiqSpsr));
    asm volatile ("mrs %0, lr_irq" : "=r" (rd->irqLr));
    asm volatile ("mrs %0, sp_irq" : "=r" (rd->irqSp));
    asm volatile ("mrs %0, spsr_irq" : "=r" (rd->irqSpsr));
    asm volatile ("mrs %0, lr_und" : "=r" (rd->undefinedLr));
    asm volatile ("mrs %0, sp_und" : "=r" (rd->undefinedSp));
    asm volatile ("mrs %0, spsr_und" : "=r" (rd->undefinedSpsr));
    return;
}


struct regDump* getRegDumpStruct(struct regDump* rd, enum ExceptionType exType, void* sp){
        getRegs(rd, sp);
        getSpecialRegs(rd);

        rd->exType = exType;

        if(exType == DATA_ABORT) {
                uint32_t dfsr = getDFSR();
                rd->accessStyle = dfsr & DFSR_RW;
                rd->faultName = ((dfsr & STATUS_LEADING) << 4) + (dfsr & 0xF);
                rd->accessAddress = getDFAR();

                rd->insAddress = rd->lr - 8;
        } else if(exType == UNDEFINED_INSTRUCTION ||
                  exType == PREFETCH_ABORT ||
                  exType == SOFTWARE_INTERRUPT){
                rd->insAddress = rd->lr - 4;
        } else {
                rd->insAddress = 0;
        }

        rd->cpsr = getCPSR();
        rd->spsr = getSPSR();




        return rd;

}
