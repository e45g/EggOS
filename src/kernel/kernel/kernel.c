#include <stdint.h>
#include <stddef.h>

#include "kernel.h"
#include "../tty.h"
#include "../vga.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you dummy."
#endif

#if !defined(__i386__)
#error "You are not using ix86-elf compiler, you dummy."
#endif

void kernel_main(void)
{
    terminal_initialization();

    terminal_writestring("Hello World!\n");
    terminal_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);

    terminal_writestring("From Kernel");

    while(1) {
        __asm__("hlt");
    }
}

