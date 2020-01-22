
#include "presentations.h"
#include "serial.h"
#include "tests.h"
#include "timer.h"
#include "handler.h"
#include "kio.h"
#include "regcheck.h"
#include "process.h"
#include "thread.h"
#include "memory.h"
#include <stdint.h>

extern void spawner();

void start_kernel(void)
{
        enableUartInterrupt();
        initMMU();
        specialMessage("Kernel loaded! (UwU)");
        /* kprintf("Erstes Byte an 0x300000: %x\n", *((uint32_t*)0x300000)); */
        /* kprintf("-----\nMMUTable Content:\n%x\n%x\n%x\n%x\n%x\n-----\n", mmuTable[0], mmuTable[1], mmuTable[2], mmuTable[3], mmuTable[4]); */
        /* remapAddressSpace(5); */
        /* kprintf("-----\nMMUTable Content (after remap to 5):\n%x\n%x\n%x\n%x\n%x\n-----\n", mmuTable[0], mmuTable[1], mmuTable[2], mmuTable[3], mmuTable[4]); */
        /* kprintf("Erstes Byte an 0x300000: %x\n", *((uint32_t*)0x300000)); */
        /* *((uint32_t*)0x300000) = 123; */
        /* kprintf("Erstes Byte an 0x300000: %x\n", *((uint32_t*)0x300000)); */
        /* map1on1(); */
        /* kprintf("-----\nMMUTable Content (after remap1on1):\n%x\n%x\n%x\n%x\n%x\n-----\n", mmuTable[0], mmuTable[1], mmuTable[2], mmuTable[3], mmuTable[4]); */
        /* kprintf("Erstes Byte an 0x300000 (after 1on1): %x\n", *((uint32_t*)0x300000)); */
        /* remapAddressSpace(5); */
        /* kprintf("-----\nMMUTable Content (after remap to 5):\n%x\n%x\n%x\n%x\n%x\n-----\n", mmuTable[0], mmuTable[1], mmuTable[2], mmuTable[3], mmuTable[4]); */
        /* kprintf("Erstes Byte an 0x300000: %x\n", *((uint32_t*)0x300000)); */
        initProcessArray();
        initTimer();
        initIdleThread();
        while(1);
}
