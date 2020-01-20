ENTRY(_start)
MEMORY
{
    KERNEL_SPACE   (rwx)   : ORIGIN = 0x008000, LENGTH = 0x200000 - 0x8000
    USER_SPACE     (rwx)   : ORIGIN = 0x200000, LENGTH = 2M
    ADDRESS_SPACES (rw)    : ORIGIN = 0x400000, LENGTH = 24M
    VIRTUAL_USER_TEXT (rx) : ORIGIN = 0x1C00000, LENGTH = 1M
}

SECTIONS
{
   	.kText                 : { *(.init .text)               } > KERNEL_SPACE
    .kData 0x100000        : { *(.rodata .bss .COMMON)      } > KERNEL_SPACE
    .uData                 : { *(.userROData .userBSSData)  } > USER_SPACE
    .uText                 : AT(0x300000) { *(.userText) } > VIRTUAL_USER_TEXT

    /*.processStorage        : { _processStorage_start = .;
                               . = _processStorage_start + 24M;
                               _processStorage_end = .;
                               } >ADDRESS_SPACES
*/
}
