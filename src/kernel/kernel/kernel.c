#include "kernel.h"
#include "../tty.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you dummy."
#endif

#if !defined(__i386__)
#error "You are not using ix86-elf compiler, you dummy."
#endif

uint32_t page_table[1024] __attribute__((aligned(4096)));
uint32_t page_directory[1024] __attribute__((aligned(4096)));

void load_page_directory(uint32_t *pd) {
    __asm__ volatile("mov %0, %%cr3" : : "r"(pd));
}

void enable_paging() {
    __asm__ volatile
    (
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
    : : : "eax");
}

void paging_setup() {
    for(unsigned int i = 0; i < 1024; i++) {
        page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.

        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x02;
    }

    page_directory[0] = ((uintptr_t)page_table) | 3;
    load_page_directory(page_directory);
}

void kernel_main(void)
{
    terminal_initialization();
    paging_setup();

    welcome_msg();

    printf("I guess paging enabled ?");

    while(1) {
        __asm__("hlt");
    }
}

