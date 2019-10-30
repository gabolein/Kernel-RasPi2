ENTRY(_start)
SECTIONS
{
	.               = 0x00008000;
	.init           : { *(.init)        }
        .trampoline     : { *(.trampoline)  }
	.text           : { *(.text)        }
        
}

 
