#include "pic.h"
#include "../common.h"

inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

void pic_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void pic_send_eoi(uint8_t irq) {
    if(irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_setup() {
    // save masks
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11);
	io_wait();
    outb(PIC2_COMMAND, 0x11);
	io_wait();
	outb(PIC1_DATA, 0x20); // master vector offset
	io_wait();
	outb(PIC2_DATA, 0x28); // slave vector offset
	io_wait();
	outb(PIC1_DATA, 0x04); // tell master that there is a slave at IRQ2
	io_wait();
	outb(PIC2_DATA, 0x02); // Slave identity
	io_wait();

	outb(PIC1_DATA, 0x01);  // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, 0x01);
	io_wait();

    // restore data
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_set_mask(uint8_t irq_line) {
    uint8_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else{
        port = PIC2_DATA;
        irq_line -= 8;
    }

    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void pic_clear_mask(uint8_t irq_line) {
    uint8_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else{
        port = PIC2_DATA;
        irq_line -= 8;
    }

    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}
