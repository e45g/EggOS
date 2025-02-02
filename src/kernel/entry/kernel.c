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
#error "You are not using ix86-elf compiler, you dummy."
#endif


void kernel_main(void)
{
    terminal_initialization();
    welcome_msg();

    pic_setup();
    idt_init();

    launch_shell();

    while(1) {
        __asm__("hlt");
    }
}

