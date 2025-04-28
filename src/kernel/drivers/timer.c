#include <cpu.h>
#include <pic.h>

static volatile uint32_t timer_ticks = 0;

void handle_timer() {
    timer_ticks++;
    pic_send_eoi(0);
}

void timer_init() {
    cli();

    //set frq

    sti();
}
