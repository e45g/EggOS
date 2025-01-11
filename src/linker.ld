OUTPUT_FORMAT(elf32-i386)
ENTRY(kernel_main)

SECTIONS
{
    . = 0x7F00;
    .text : { *(.text.start) *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss) *(COMMON) }
}
