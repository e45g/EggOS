#include <keyboard.h>
#include <tty.h>
#include <pic.h>

char kb[] = {
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
    0, // scrolllock
    7, // keypad 7
    8, // keypad 8
    9, // keypad 9
    '-', // keypad -
    4, // keypad 4
    5, // keypad 5
    6, // keypad 6
    '+', // keypad +
    1, // keypad 1
    2, // keypad 2
    3, // keypad 3
    0, // keypad 0
    '.', // keypad .
};

char kb_shift[] = {
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
    0, // scrolllock
    7, // keypad 7
    8, // keypad 8
    9, // keypad 9
    '-', // keypad -
    4, // keypad 4
    5, // keypad 5
    6, // keypad 6
    '+', // keypad +
    1, // keypad 1
    2, // keypad 2
    3, // keypad 3
    0, // keypad 0
    '.', // keypad .
};

static keyboard_t keyboard = {
    .mod_keys = MOD_NONE,
    .state = STATE_NORMAL,
    .buffer = {0},
    .head = 0,
    .tail = 0
};

static void enqueue_char(char c) {
    if (c != 0) {
        keyboard.buffer[keyboard.tail] = c;
        keyboard.tail = (keyboard.tail + 1) % KB_BUFFER_MAX;
    }
}

static bool handle_modifier(uint8_t scancode) {
    switch (scancode) {
        case LSHIFT_PRESSED:
            keyboard.mod_keys |= MOD_SHIFT;
            return true;
        case LCTRL_PRESSED:
            keyboard.mod_keys |= MOD_CTRL;
            return true;
        case LALT_PRESSED:
            keyboard.mod_keys |= MOD_ALT;
            return true;
        case LSHIFT_PRESSED + 0x80:
            keyboard.mod_keys &= ~MOD_SHIFT;
            return true;
        case LCTRL_PRESSED + 0x80:
            keyboard.mod_keys &= ~MOD_CTRL;
            return true;
        case LALT_PRESSED + 0x80:
            keyboard.mod_keys &= ~MOD_ALT;
            return true;
        default:
            return false;
    }
}

static bool handle_extended(uint8_t scancode) {
    char c = 0;
    switch (scancode) {
        case 0x4D:  // right arrow
            c = RIGHT_ARROW_PRESSED;
            break;
        case 0x4B:  // left arrow
            c = LEFT_ARROW_PRESSED;
            break;
        case 0x48:  // up arrow
            c = UP_ARROW_PRESSED;
            break;
        case 0x50:  // down arrow
            c = DOWN_ARROW_PRESSED;
            break;
        default:
            return false;
    }
    enqueue_char(c);
    keyboard.state = STATE_NORMAL;
    return true;
}

void handle_keyboard() {
    uint8_t scancode = inb(0x60);
    char c = 0;

    if (handle_modifier(scancode)) {
        goto finish;
    }

    if(scancode == 0xE0) {
        keyboard.state = STATE_EXTENDED;
        goto finish;
    }

    if (keyboard.state == STATE_EXTENDED) {
        if (handle_extended(scancode)) {
            goto finish;
        }
        keyboard.state = STATE_NORMAL;
    }

    if (scancode >= sizeof(kb)) {
        goto finish;
    }

    if (keyboard.mod_keys & MOD_SHIFT) {
        c = kb_shift[scancode];
    }
    else if(keyboard.mod_keys & MOD_CTRL) {
        if(kb[scancode] == 'l') terminal_clear();
        goto finish;
    }
    else {
        c = kb[scancode];
    }

    enqueue_char(c);


finish:
    pic_send_eoi(1);
    return;
}

uint8_t getchar(void) {
    while(keyboard.head == keyboard.tail) {
        __asm__ volatile("hlt");
    }
    unsigned c = keyboard.buffer[keyboard.head];
    keyboard.head = (keyboard.head + 1) % KB_BUFFER_MAX;
    if(c == UP_ARROW_PRESSED) printf("heeeh");
    return c;
}

char *getstr(char *buf, uint16_t len) {
    uint16_t i = 0;
    int c;

    while(i < len - 1) {
        c = getchar();

        if(c == 0) {
            if (i == 0) {
                return NULL;
            }
            break;
        }

        if(c == '\n') {
            buf[i++] = '\0';
            terminal_putchar(c);
            terminal_update_cursor();
            break;
        }

        if(c == '\b') {
            if(i > 0) {
                i--;
                terminal_putchar(c);
                terminal_update_cursor();
            }
            continue;
        }

        buf[i++] = c;
        terminal_putchar(c);
        terminal_update_cursor();

    }
    return buf;
}
