#pragma once

#include <common.h>

#define MOD_NONE    0
#define MOD_CTRL    (1 << 0)
#define MOD_SHIFT   (1 << 1)
#define MOD_ALT     (1 << 2)

#define ESC_PRESSED         0x1
#define LCTRL_PRESSED       0x1D
#define LSHIFT_PRESSED      0x2A
#define RSHIFT_PRESSED      0x36
#define LALT_PRESSED        0x38

#define LEFT_ARROW_PRESSED  0x80
#define UP_ARROW_PRESSED    0x81
#define RIGHT_ARROW_PRESSED 0x82
#define DOWN_ARROW_PRESSED  0x83

#define STATE_NORMAL 0
#define STATE_EXTENDED 1

#define KB_BUFFER_MAX 128

typedef struct {
    unsigned char mod_keys;
    unsigned int state;
    char buffer[KB_BUFFER_MAX];
    int head;
    int tail;
} keyboard_t;

uint8_t getchar(void);
char *getstr(char *buf, uint16_t len);

