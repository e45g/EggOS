ENTRY(_start)
SECTIONS
{
    . = 0x00100000;
    .text : ALIGN(4096)
    {
        *(.text)
    }

    /*  Read-only data */
    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    /*  Read-write data (initialized) */
    .data : ALIGN(4096)
    {
        *(.data)
    }

    /*  Read-write data (uninitialized) and stack */
    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }
}
