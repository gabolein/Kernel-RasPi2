#include <stdint.h>

#ifndef _PRESENTATIONS_H
#define _PRESENTATIONS_H

enum AccessStyle {
        READ,
        WRITE,
        NONE
};

enum ExceptionType {
        SOFTWARE_INTERRUPT,
        DATA_ABORT,
        UNDEFINED_INSTRUCTION,
        PREFETCH_ABORT,
        IRQ,
        FIQ
};

struct regDump {
        enum ExceptionType exType;
        uint32_t insAddress;
        enum AccessStyle accessStyle; 
        uint32_t accessAddress;
        char* faultName;
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

void inputPresentation(char receivedChar);
void specialMessage(const char*);

void printRegisterDump(struct regDump*);

#endif
