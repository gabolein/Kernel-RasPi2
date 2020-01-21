ENTRY(_start)
MEMORY
{
    KERNEL_TEXT   (rx)   : ORIGIN = 0x008000, LENGTH = 0x100000 - 0x8000
    KERNEL_DATA   (rw)   : ORIGIN = 0x100000, LENGTH = 1M
}

SECTIONS
{
    .kInit                 : { *(.init)                     } > KERNEL_TEXT
   	.kText                 : { *(.text .rodata)             } > KERNEL_TEXT
    .kData 0x100000        : { *(.bss .data)                } > KERNEL_DATA

    .uText 0x200000        : { *(.userText) *(.userROData)  }
    .uData 0x300000        : { *(.userBSSData .userData)    }
}
