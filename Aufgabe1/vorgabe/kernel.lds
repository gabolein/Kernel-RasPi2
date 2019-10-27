ENTRY(_start)
SECTIONS
{
	. = 0x00008000;
        .ivt  : { *(.ivt)  }
	.init : { *(.init) }
	.text : { *(.text) }
}

