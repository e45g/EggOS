#include "memory.h"
#include "utils.h"
#include <kernel.h>
#include <tty.h>
#include <idt.h>
#include <pic.h>
#include <keyboard.h>
#include <shell.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you dummy."
#endif

#if !defined(__i386__)
#error "You are not using x86-elf compiler, you dummy."
#endif

uintptr_t kernel_end;

void kmain(void)
{
    uintptr_t kernel_end_addr = (uintptr_t)&_kernel_end;
    kernel_end = ALIGN_UP(kernel_end_addr, PAGE_SIZE);

    terminal_initialization();
    welcome_msg();

    pic_setup();
    idt_init();

    pmm_init();
    vmm_init();

    heap_init();

    launch_shell();

    while(1) {
        __asm__("hlt");
    }
}

