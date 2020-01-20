SECTIONS
{
    .userROData             : { *(.rodata) }
    .userBSSData            : { *(.bss)    }
    .userText               : { *(.text) }
}
