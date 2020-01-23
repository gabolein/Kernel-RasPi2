KERNEL_DATA_START = (0x100000);
USER_TEXT         = (0x200000);
USER_DATA         = (0x300000);
KERNEL_MEM_START  = (0x8000);

ENTRY(_start)
MEMORY
{
    KERNEL_TEXT   (rx)   : ORIGIN = KERNEL_MEM_START, LENGTH = KERNEL_DATA_START - KERNEL_MEM_START
    KERNEL_DATA   (rw)   : ORIGIN = KERNEL_DATA_START, LENGTH = 1M
}

SECTIONS
{
    .kInit                   : { *(.init)                                 } > KERNEL_TEXT
   	.kText                   : { *(.text)                                 } > KERNEL_TEXT
    .kData KERNEL_DATA_START : { *(.bss .data .rodata*)                   } > KERNEL_DATA

    .uText USER_TEXT         : { *(.userText)                             }
    .uData USER_DATA         : { *(.userBSSData .userData .userROData*)   }
}
