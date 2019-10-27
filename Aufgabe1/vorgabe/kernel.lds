ENTRY(_start)
SECTIONS
{
	.               = 0x00008000;
        .ivt            : { *(.ivt)         }
        .trampoline     : { *(.trampoline)  }
	.init           : { *(.init)        }
	.text           : { *(.text)        }
        
}

 
