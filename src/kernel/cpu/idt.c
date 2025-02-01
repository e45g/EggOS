#include <idt.h>
#include <common.h>
#include <utils.h>
#include <vga.h>
#include <tty.h>
#include <keyboard.h>
#include <pic.h>

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];
static idtr_t idtr;

__attribute__((noreturn))
void exception_handler(uint32_t vector, uint32_t error_code) {
    terminal_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    uintptr_t esp = get_esp();

    printf("Exception: %d\nError Code: %d\nESP: %p\n", vector, error_code, esp);

    while(1) {
        __asm__ volatile("cli; hlt");
    }
}


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uintptr_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uintptr_t)isr >> 16;
    descriptor->reserved = 0;
}


static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for(uint32_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    idt_set_descriptor(32, isr_stub_table[32], 0x8E); // timer
    vectors[32] = true;

    idt_set_descriptor(33, isr_stub_table[33], 0x8E); // keyboard
    vectors[33] = true;

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");
}
