#include "bootinfo.h"
#include "memory.h"
#include "utils.h"
#include <kernel.h>
#include <idt.h>
#include <pic.h>
#include <keyboard.h>
#include <shell.h>
#include <tty.h>
#include <tests.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you dummy."
#endif

#if !defined(__i386__)
#error "You are not using x86-elf compiler, you dummy."
#endif

uintptr_t kernel_end;
uintptr_t kernel_start;
bootinfo_t boot_info;

void kmain(void)
{
    uintptr_t kernel_end_addr = (uintptr_t)&_kernel_end;
    uintptr_t kernel_start_addr = (uintptr_t)&_kernel_start;
    kernel_end = ALIGN_UP(kernel_end_addr, PAGE_SIZE);
    kernel_start = ALIGN_UP(kernel_start_addr, PAGE_SIZE);

    terminal_initialization();
    welcome_msg();

    get_bootinfo(&boot_info);
    if(boot_info.magic != 0x12345678) {
        PANIC("Boot info is not loaded correctly!");
    }

    pic_setup();
    idt_init();

    pmm_init();
    vmm_init();

    heap_init();

    // run_memory_tests();

    launch_shell();

    while(1) {
        __asm__("hlt");
    }
}

