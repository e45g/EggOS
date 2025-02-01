#include "common.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define MOD_NONE 0
#define MOD_CTRL 1 << 0
#define MOD_SHIFT 1 << 1
#define MOD_ALT 1 << 2

#define ESC_PRESSED 0x1
#define LCTRL_PRESSED 0x1D
#define LSHIFT_PRESSED 0x2A
#define RSHIFT_PRESSED 0x36
#define LALT_PRESSED 0x38

#define KEYBOARD_BUFFER_MAX 128

static char keyboard[] = {
    0, // nothing
    27, // esc
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', //backspace
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, // left control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, // left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // right shift
    '*', // keypad
    0, // left alt
    ' ', 0, // caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // f0 - f10 keys
    0, // numlock
    0 // scrolllock
};

static char keyboard_shift[] = {
    0, // nothing
    27, // esc
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', //backspace
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, // left control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, // left shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, // right shift
    '*', // keypad
    0, // left alt
    ' ', 0, // caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // f0 - f10 keys
    0, // numlock
    0 // scrolllock
};

static char keyboard_alt[] = {
    0, // nothing
    27, // esc
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', //backspace
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, // left control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, // left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // right shift
    '*', // keypad
    0, // left alt
    ' ', 0, // caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // f0 - f10 keys
    0, // numlock
    0 // scrolllock
};

char getchar(void);

#endif
