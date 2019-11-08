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

/* Get LR of Modes */
static inline uint32_t getLR_USR() {
        uint32_t lr_usr = 0;
        asm volatile ("mrs %0, lr_usr": "=r" (lr_usr));
        return lr_usr;
}

static inline uint32_t getLR_SVC() {
        uint32_t lr_svc = 0;
        asm volatile ("mrs %0, lr_svc": "=r" (lr_svc));
        return lr_svc;
}

static inline uint32_t getLR_ABT() {
        uint32_t lr_abt = 0;
        asm volatile ("mrs %0, lr_abt": "=r" (lr_abt));
        return lr_abt;
}

static inline uint32_t getLR_FIQ() {
        uint32_t lr_fiq = 0;
        asm volatile ("mrs %0, lr_fiq": "=r" (lr_fiq));
        return lr_fiq;
}

static inline uint32_t getLR_IRQ() {
        uint32_t lr_irq = 0;
        asm volatile ("mrs %0, lr_irq": "=r" (lr_irq));
        return lr_irq;
}

static inline uint32_t getLR_UND() {
        uint32_t lr_und = 0;
        asm volatile ("mrs %0, lr_und": "=r" (lr_und));
        return lr_und;
}

/* Get SP of modes */
static inline uint32_t getSP_USR() {
        uint32_t sp_usr = 0;
        asm volatile ("mrs %0, sp_usr": "=r" (sp_usr));
        return sp_usr;
}

static inline uint32_t getSP_SVC() {
        uint32_t sp_svc = 0;
        asm volatile ("mrs %0, sp_svc": "=r" (sp_svc));
        return sp_svc;
}

static inline uint32_t getSP_ABT() {
        uint32_t sp_abt = 0;
        asm volatile ("mrs %0, sp_abt": "=r" (sp_abt));
        return sp_abt;
}

static inline uint32_t getSP_FIQ() {
        uint32_t sp_fiq = 0;
        asm volatile ("mrs %0, sp_fiq": "=r" (sp_fiq));
        return sp_fiq;
}

static inline uint32_t getSP_IRQ() {
        uint32_t sp_irq = 0;
        asm volatile ("mrs %0, sp_irq": "=r" (sp_irq));
        return sp_irq;
}

static inline uint32_t getSP_UND() {
        uint32_t sp_und = 0;
        asm volatile ("mrs %0, sp_und": "=r" (sp_und));
        return sp_und;
}

/* Get SPSR for every mode */

static inline uint32_t getSPSR_SVC() {
        uint32_t spsr_svc = 0;
        asm volatile ("mrs %0, spsr_svc": "=r" (spsr_svc));
        return spsr_svc;
}

static inline uint32_t getSPSR_ABT() {
        uint32_t spsr_abt = 0;
        asm volatile ("mrs %0, spsr_abt": "=r" (spsr_abt));
        return spsr_abt;
}

static inline uint32_t getSPSR_FIQ() {
        uint32_t spsr_fiq = 0;
        asm volatile ("mrs %0, spsr_fiq": "=r" (spsr_fiq));
        return spsr_fiq;
}

static inline uint32_t getSPSR_IRQ() {
        uint32_t spsr_irq = 0;
        asm volatile ("mrs %0, spsr_irq": "=r" (spsr_irq));
        return spsr_irq;
}

static inline uint32_t getSPSR_UND() {
        uint32_t spsr_und = 0;
        asm volatile ("mrs %0, spsr_und": "=r" (spsr_und));
        return spsr_und;
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
                kprintf("    (%x)\n", spsr); /* TODO: Problem, brauchen getPSR String
        }
}

void getRegs(struct regDump* rd, void* sp){
        struct commonRegs* regs = sp;
        *((struct commonRegs*)rd) = *regs;
        return;
}


struct regDump* getRegDumpStruct(struct regDump* rd, enum ExceptionType exType, void* sp){
        getRegs(rd, sp);

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
        rd->userLr = getLR_USR();
        rd->userSp = getSP_USR();
        rd->supervisorLr = getLR_SVC();
        rd->supervisorSp = getSP_SVC();
        rd->supervisorSpsr = getSPSR_SVC();
        rd->abortLr = getLR_ABT();
        rd->abortSp = getSP_ABT();
        rd->abortSpsr = getSPSR_ABT();
        rd->fiqLr = getLR_FIQ();
        rd->fiqSp = getSP_FIQ();
        rd->fiqSpsr = getSPSR_FIQ();
        rd->irqLr = getLR_IRQ();
        rd->irqSp = getSP_IRQ();
        rd->irqSpsr = getSPSR_IRQ();
        rd->undefinedLr = getLR_UND();
        rd->undefinedSp = getSP_UND();
        rd->undefinedSpsr = getSPSR_UND();

        return rd;

}
