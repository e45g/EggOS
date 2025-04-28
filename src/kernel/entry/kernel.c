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

    char *c = malloc(13);
    if(c == NULL) return;
    memset(c, 'a', 12);
    c[12] = '\0';
    printf("%s\n", c);

    char *c1 = malloc(120);
    if(c1 == NULL) return;
    memset(c1, 'b', 12);
    c1[12] = '\0';
    printf("%s\n", c1);

    free(c);

    char *c2 = malloc(120);
    if(c2 == NULL) return;
    memset(c2, 'c', 12);
    c2[12] = '\0';
    printf("%s\n", c2);

    free(c1);
    free(c2);


    launch_shell();

    while(1) {
        __asm__("hlt");
    }
}

