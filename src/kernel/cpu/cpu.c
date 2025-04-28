#include <common.h>

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void io_wait(void) {
    outb(0x80, 0);
}

void cli(void) {
    __asm__ volatile("cli");
}

void sti(void) {
    __asm__ volatile("sti");
}
