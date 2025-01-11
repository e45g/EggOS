#include <stdint.h>
#include <stddef.h>

#include "kernel.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you dummy."
#endif

#if !defined(__i386__)
#error "You are not using ix86-elf compiler, you dummy."
#endif


enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
void kernel_main(void)
{

    while(1) {
        __asm__("hlt");
    }
}

