#pragma once

#include "common.h"

/*
* Initializes the terminal:
*  - set position to 0, 0
*  - clears terminal
*  - sets color to white on black
*/
void terminal_initialization();

/*
* Scrolls up(0) or down(1)
* no direction for now
*/
void terminal_scroll();

/*
* Puts a character `c` to a position `x, y` with color `color`.
*/
void terminal_putcharat(char c, uint8_t color, uint16_t x, uint16_t y);

/*
* Puts a character `c` at the current position.
*/
void terminal_putc(char c);

/*
* Writes string `str` with size of `size` at the current position.
*/
void terminal_write(const char *str, uint16_t size);

/*
* Writes a string `str` at the current position.
*/
void terminal_writestring(const char *str);

/*
* Sets terminal color.
*/
void terminal_set_color(uint8_t fg, uint8_t bg);

/*
* :) Kernel OK
*/
void welcome_msg();

/*
* Update the cursor position.
*/
void terminal_update_cursor();


/*
* Move cursor relatively to the current position.
*/
void terminal_rmove(int x, int y);

/*
* Clear the screen.
*/
void terminal_clear();


/*
* Sets the cursor position.
*/
void terminal_set_cursor(uint16_t x, uint16_t y);

/*
* Clear the current line.
*/
void terminal_clear_line();
