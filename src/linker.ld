ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0x7F00;
    .text : { *(.text.start) *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) *(COMMON) }
}
ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0x7F00;
    .text : { *(.text.start) *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) *(COMMON) }
}
