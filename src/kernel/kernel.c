#include "kernel.h"

#if defined (__linux__)
#error "Use a cross-compiler you dummy."
#endif

#if !defined(__i386__)
#error "Use ix86-elf compiler dummy."
#endif

void kernel_main() {
    char *video_memory = (char *)0xB8000;
    video_memory[0] = 'K'; // First character
    video_memory[1] = 0x07; // White on black
    video_memory[2] = 'E'; // Second character
    video_memory[3] = 0x07;
    video_memory[4] = 'R'; // Third character
    video_memory[5] = 0x07;
}
