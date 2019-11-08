#ifndef REGISTER_DUMP_UTIL_H
#define REGISTER_DUMP_UTIL_H

enum SFSRStatus {
        ALIGNMENT_FAULT                      = 0x1,
        DEBUG_EVENT                          = 0x2,
        ACCESS_FLAG_FAULT_ON_SECTION         = 0x3,
        CACHE_MAINTENANCE_FAULT              = 0x4,
        TRANSLATION_FAULT_ON_SECTION         = 0x5,
        ACCESS_FLAG_FAULT_ON_PAGE            = 0x6,
        TRANSLATION_FAULT_ON_PAGE            = 0x7,
        PRECISE_EXTERNAL_ABORT               = 0x8,
        DOMAIN_FAULT_ON_SECTION              = 0x9,
        DOMAIN_FAULT_ON_PAGE                 = 0xB,
        EXTERNAL_ABORT_ON_TRANSLATION_FIRST  = 0xC,
        PERMISSION_FAULT_ON_SECTION          = 0xD,
        EXTERNAL_ABORT_ON_TRANSLATION_SECOND = 0xE,
        PERMISSION_FAULT_ON_PAGE             = 0xF,
        IMPRECISE_EXTERNAL_ABORT             = 0x16
};

enum AccessStyle {
        READ = 0,
        WRITE = 1,
        NONE = 2
};

enum Modes {
        USER_MODE = 0x10,
        FAST_INTERRUPT_MODE = 0x11,
        IRQ_MODE = 0x12,
        SUPERVISOR_MODE = 0x13,
        MONITOR_MODE = 0x16,
        ABORT_MODE = 0x17,
        HYPERVISOR_MODE = 0x1A,
        UNDEFINED_MODE = 0x1B,
        SYSTEM_MODE = 0x1F
};

enum ExceptionType {
        SOFTWARE_INTERRUPT,
        DATA_ABORT,
        UNDEFINED_INSTRUCTION,
        PREFETCH_ABORT,
        IRQ,
        FIQ
};

struct commonRegs {
        /* Die müssen hier oben sein, weil sie in ivt.S in
           dieser Reihenfolge auf den Stack geschrieben werden */
        uint32_t sp;
        uint32_t lr;
        uint32_t pc;

        uint32_t r0;
        uint32_t r1;
        uint32_t r2;
        uint32_t r3;
        uint32_t r4;
        uint32_t r5;
        uint32_t r6;
        uint32_t r7;
        uint32_t r8;
        uint32_t r9;
        uint32_t r10;
        uint32_t r11;
        uint32_t r12;
};

struct regDump {
        enum ExceptionType exType;
        uint32_t insAddress; /* Address of exception causing instruction */

        /* Data Abort stuff begin */
        enum AccessStyle accessStyle; //DFSR Register
        uint32_t accessAddress; //DFAR Register
        enum SFSRStatus faultName;
        /* Data Abort stuff end */

        uint32_t r0;
        uint32_t r1;
        uint32_t r2;
        uint32_t r3;
        uint32_t r4;
        uint32_t r5;
        uint32_t r6;
        uint32_t r7;
        uint32_t r8;
        uint32_t r9;
        uint32_t r10;
        uint32_t r11;
        uint32_t r12;
        uint32_t sp;
        uint32_t lr;
        uint32_t pc;
        uint32_t cpsr;
        uint32_t spsr;
        uint32_t userLr;
        uint32_t userSp;
        uint32_t supervisorLr;
        uint32_t supervisorSp;
        uint32_t supervisorSpsr;
        uint32_t abortLr;
        uint32_t abortSp;
        uint32_t abortSpsr;
        uint32_t fiqLr;
        uint32_t fiqSp;
        uint32_t fiqSpsr;
        uint32_t irqLr;
        uint32_t irqSp;
        uint32_t irqSpsr;
        uint32_t undefinedLr;
        uint32_t undefinedSp;
        uint32_t undefinedSpsr;
};

/* Gets a char[11] and a PSR Register and fills the string
   with a representation of the psr  */
char* getPSRStrings(uint32_t, char*);

/* ModeType, Mode LR, Mode SP, Mode SPSR (for User/System doesn't matter)  */
void printModeRegs(enum Modes, uint32_t, uint32_t, uint32_t);

void printMode(enum Modes);

/* Creates a regDump struct at pointer */
struct regDump* getRegDumpStruct(struct regDump*, enum ExceptionType, void*);

void getRegs(struct regDump*, void*);


#endif
