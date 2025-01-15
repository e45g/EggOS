#include "kernel.h"
#include "../tty.h"
#include "idt.h"
#include "pic.h"

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

        // supervisor level, read/write, not present
        page_directory[i] = 0x02;
    }

    page_directory[0] = ((uintptr_t)page_table) | 3;
    load_page_directory(page_directory);
}

uintptr_t get_esp(void) {
    uintptr_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    return esp;
}

void print_stack(uintptr_t *stack, int entries) {
    for(int i = 0; i < entries; i++) {
        printf("%p : %x\n", stack+i, stack[i]);
    }
}


void kernel_main(void)
{
    terminal_initialization();
    welcome_msg();

    uintptr_t esp = get_esp();
    printf("Address of ESP: %p\n", (void *)esp);

    paging_setup();
    printf("I guess paging enabled ?\n");

    pic_setup();
    pic_disable();
    pic_clear_mask(1);

    // pic_clear_mask(0);

    idt_init();
    printf("I guess idt init went well?\n");

    while(1) {
        __asm__("hlt");
    }
}

