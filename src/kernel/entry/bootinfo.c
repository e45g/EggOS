#include "kernel.h"
#include <bootinfo.h>

void get_bootinfo(bootinfo_t *info) {
    bootinfo_t *bootinfo_ptr;

    asm __volatile__(
        "movl %%ebx, %0"
        : "=r" (bootinfo_ptr)
        :
        : "memory"
    );

    if (bootinfo_ptr) {
        *info = *bootinfo_ptr;
    }

    if(info->magic != 0x12345678) {
        PANIC("Boot info is not loaded correctly!");
    }
}
