#pragma once

#include <common.h>

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

char getchar(void);
char *getstr(char *buf, uint16_t len);
