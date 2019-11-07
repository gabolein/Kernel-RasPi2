#include <stdint.h>

#ifndef _BAR_H_
#define _BAR_H_

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


extern void undefined_instruction();
extern void software_interrupt();
extern void prefetch_abort();
extern void data_abort();
extern void not_used();
extern void irq();
extern void fiq();





#endif

