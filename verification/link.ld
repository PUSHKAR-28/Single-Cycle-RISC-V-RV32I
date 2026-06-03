MEMORY
{
    IMEM (rx)  : ORIGIN = 0x00000000, LENGTH = 16K
    DMEM (rw)  : ORIGIN = 0x00000000, LENGTH = 16K
}

SECTIONS
{
    .text 0x00000000 :
    {
        *(.text.init)
        *(.text*)
        
    }
    .data 0x00004000 :
    {
        *(.data*)
	*(.rodata*)
        *(.bss*)
        *(.sdata*)
    }
}

