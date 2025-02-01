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
    0 // scrolllock
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
    0 // scrolllock
};

static unsigned char mod_keys = 0;

static char kb_buffer[KEYBOARD_BUFFER_MAX] = {0};
static int kb_head = 0;
static int kb_tail = 0;

void handle_keyboard() {
    uint8_t scancode = inb(0x60);
    char c;

    switch(scancode) {
        case LSHIFT_PRESSED:
            mod_keys |= MOD_SHIFT;
            goto mod;
        case LCTRL_PRESSED:
            mod_keys |= MOD_CTRL;
            goto mod;
        case LALT_PRESSED:
            mod_keys |= MOD_ALT;
            goto mod;

        case LSHIFT_PRESSED + 0x80:
            mod_keys &= ~(MOD_SHIFT);
            goto mod;
        case LCTRL_PRESSED + 0x80:
            mod_keys &= ~(MOD_CTRL);
            goto mod;
        case LALT_PRESSED + 0x80:
            mod_keys &= ~(MOD_ALT);
            goto mod;
    }

    if(scancode > sizeof(kb)) {
        goto mod;
    }

    if(mod_keys == MOD_NONE && !(scancode & 0x80)) {
        c = kb[scancode];
    }
    else if(mod_keys == MOD_SHIFT && !(scancode & 0x80)) {
        c = kb_shift[scancode];
    }
    else if(mod_keys == MOD_CTRL && !(scancode & 0x80)) {
        c = 4;
    }
    else if ( !(scancode & 0x80) ) {
		// printf("Not implemented (scancode = %x)\n", scancode);
        c = 1;
	}

    if (c != 0) {
        kb_buffer[kb_head] = c;
        kb_head = (kb_head + 1) % KEYBOARD_BUFFER_MAX;
    }

mod:
    pic_send_eoi(1);
    return;
}

char getchar(void) {
    while(kb_head == kb_tail) {
        __asm__ volatile("hlt");
    }
    char c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KEYBOARD_BUFFER_MAX;
    return c;
}

char *getstr(char *buf, uint16_t len) {
    int c;
    uint16_t i = 0;

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
