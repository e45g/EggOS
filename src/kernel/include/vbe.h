// #pragma once
//
// #include <common.h>
//
// #define SCREEN_WIDTH 1280
// #define SCREEN_HEIGHT 1024
// #define BPP 16
// #define BYTES_PER_PIXEL (BPP/8)
// #define PITCH (SCREEN_WIDTH * BYTES_PER_PIXEL)
//
// #define FONT_WIDTH 8
// #define FONT_HEIGHT 16
//
// #define VBE_FRAMEBUFFER_VADDR 0x200000000
//
// extern uint8_t font8x16[];
//
// void vbe_init();
// void vbe_map();
// void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
// uint16_t create_color(uint8_t red, uint8_t green, uint8_t blue);
//
// void draw_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t c);
// void draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint16_t color);
//
//
// void set_fg(uint8_t r, uint8_t g, uint8_t b);
// void set_bg(uint8_t r, uint8_t g, uint8_t b);
// void fb_putc(char c);
// void fb_puts(const char *str);
//
// void terminal_update_cursor();
// void terminal_clear_line();
// void fb_clear();
