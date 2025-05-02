// #include "cpu.h"
// #include <kernel.h>
// #include "memory.h"
// #include <common.h>
// #include <vbe.h>
//
// static uint32_t *fb_ptr;
// static uintptr_t frame_buffer;
//
// static uint32_t cursor_x;
// static uint32_t cursor_y;
//
// static uint16_t fg;
// static uint16_t bg;
//
// void vbe_init() {
//     fb_ptr = (uint32_t *)0x9F00;
//     frame_buffer = *fb_ptr;
//
//     fg = create_color(255, 255, 255);
//     bg = create_color(0, 0, 0);
//     cursor_x = 0;
//     cursor_y = 0;
//
//     memset((uint32_t *)frame_buffer, 0, PITCH * SCREEN_HEIGHT);
//     fb_puts("hello wrld");
// }
//
// void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
//     if (x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
//         return;
//     uint32_t offset = (y * SCREEN_WIDTH + x) * BYTES_PER_PIXEL;
//     *((uint32_t *)(frame_buffer + offset)) = color;
// }
//
// uint16_t create_color(uint8_t red, uint8_t green, uint8_t blue) {
//     uint16_t r = (red >> 3) & 0x1F;
//     uint16_t g = (green >> 2) & 0x3F;
//     uint16_t b = (blue >> 3) & 0x1F;
//
//     return (r << 11) | (g << 5) | b;
// }
//
// static void draw_char(char c, uint32_t x, uint32_t y, uint16_t color_fg,
//                       uint16_t color_bg) {
//     const uint8_t *glyph = &font8x16[(uint8_t)c * FONT_HEIGHT];
//     uint16_t px = x * FONT_WIDTH;
//     uint16_t py = y * FONT_HEIGHT;
//     for (uint32_t row = 0; row < FONT_HEIGHT; row++) {
//         uint8_t bits = glyph[row];
//         for (uint32_t col = 0; col < FONT_WIDTH; col++) {
//             x = px + col;
//             y = py + row;
//
//             if (bits & (1 << (7 - col))) {
//                 draw_pixel(x, y, color_fg);
//             } else {
//                 draw_pixel(x, y, color_bg);
//             }
//         }
//     }
// }
//
// void fb_putc(char c) {
//     if (c == '\n') {
//         cursor_x = 0;
//         cursor_y++;
//     } else if (c == '\r') {
//         cursor_x = 0;
//     } else {
//         draw_char(c, cursor_x, cursor_y, fg, bg);
//         cursor_x++;
//         if (cursor_x >= SCREEN_WIDTH / FONT_WIDTH) {
//             cursor_x = 0;
//             cursor_y++;
//         }
//     }
//     if ((cursor_y + 1) * FONT_HEIGHT > SCREEN_HEIGHT) {
//         // TODO: scroll
//     }
// }
//
// void fb_puts(const char *str) {
//     while (*str)
//         fb_putc(*str++);
//     terminal_update_cursor();
// }
//
// void terminal_update_cursor() {
//     uint16_t pos = cursor_y * SCREEN_HEIGHT + cursor_x;
//
//     outb(0x3D4, 0x0F);
//     outb(0x3D5, (uint8_t)(pos & 0xFF));
//     outb(0x3D4, 0x0E);
//     outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
// }
//
// void set_fg(uint8_t r, uint8_t g, uint8_t b) {
//     fg = create_color(r, g, b);
// }
// void set_bg(uint8_t r, uint8_t g, uint8_t b) {
//     bg = create_color(r, g, b);
// }
//
// void fb_clear() {
//     memset((uint32_t *)frame_buffer, 0, PITCH * SCREEN_HEIGHT);
// }
//
// void terminal_clear_line() {
//     memset((uint32_t *)frame_buffer + cursor_y, 0, PITCH);
// }
//
// void draw_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
//                uint16_t color) {
//     for (uint32_t y = y1; y <= y2; y++) {
//         for (uint32_t x = x1; x <= x2; x++) {
//             draw_pixel(x, y, color);
//         }
//     }
// }
//
// void draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint16_t color) {
//     int32_t x = r;
//     int32_t y = 0;
//     int32_t err = 0;
//
//     while (x >= y) {
//         for (int32_t i = (int32_t)cx - x; i <= (int32_t)cx + x; i++) {
//             draw_pixel(i, cy + y, color);
//             draw_pixel(i, cy - y, color);
//         }
//
//         for (int32_t i = (int32_t)cx - y; i <= (int32_t)cx + y; i++) {
//             draw_pixel(i, cy + x, color);
//             draw_pixel(i, cy - x, color);
//         }
//
//         y++;
//
//         if (err <= 0) {
//             err += 2 * y + 1;
//         }
//
//         if (err > 0) {
//             x--;
//             err -= 2 * x + 1;
//         }
//     }
// }
