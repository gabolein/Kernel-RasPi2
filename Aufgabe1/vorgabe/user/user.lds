SECTIONS
{
    .userROData             : { *(.rodata*) }
    .userBSSData            : { *(.bss)    }
    .userText               : { *(.text)   }
    .userData               : { *(.data)   }
}
