SECTIONS
{
    .                       = 0x200000;
    .userROData             : { *(.rodata) }
    .userBSSData            : { *(.bss)    }
    .userText 0x300000      : { *(.text) }
}
