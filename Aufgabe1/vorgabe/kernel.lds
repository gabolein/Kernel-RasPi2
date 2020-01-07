ENTRY(_start)
SECTIONS
{
	.               = 0x00008000;
   	.init           : { *(.init)        }
	.text           : { *(.text)        }
    .user 0x100000   : { *(.user)        }
}
