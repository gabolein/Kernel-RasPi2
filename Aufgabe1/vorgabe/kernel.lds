ENTRY(_start)
SECTIONS
{
	.               = 0x00008000;
        .ivt            : ALIGN(0x20)
        { *(.ivt)         }
        .trampoline     : { *(.trampoline)  }
	.init           : { *(.init)        }
	.text           : { *(.text)        }
        
}

 
