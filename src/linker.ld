ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0x10000;
    .text : { *(.text.start) *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) *(COMMON) }
}

