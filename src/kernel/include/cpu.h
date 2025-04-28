#pragma once

#include <common.h>

void io_wait(void);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

void cli(void);
void sti(void);
