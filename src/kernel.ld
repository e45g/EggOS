ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0x100000;
    _kernel_start = .;

    .text : {
        *(.text.start)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    bss_start = .;
    .bss : {
        *(.bss*)
        *(COMMON)
    }
    bss_end = .;
    _kernel_end = .;
}
