ENTRY(_start)
MEMORY
{
    KERNEL_SPACE   (rwx) : ORIGIN = 0x008000, LENGTH = 0x200000 - 0x8000
    USER_SPACE     (rwx) : ORIGIN = 0x200000, LENGTH = 2M
    ADDRESS_SPACES (rw)  : ORIGIN = 0x400000, LENGTH = 24M
}

SECTIONS
{
   	.kText                 : { *(.init .text)               } >KERNEL_SPACE
    .kData 0x100000        : { *(.rodata .bss .COMMON)      } >KERNEL_SPACE
    .uData                 : { *(.userROData .userBSSData)  } >USER_SPACE
    .uText 0x300000        : { *(.userText) } >USER_SPACE
    .processStorage        : { } > ADDRESS_SPACES
}
