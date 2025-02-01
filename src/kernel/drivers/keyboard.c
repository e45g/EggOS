#include <keyboard.h>
#include <tty.h>
#include <pic.h>

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

    if(scancode > sizeof(keyboard)) {
        goto mod;
    }

    if(mod_keys == MOD_NONE && !(scancode & 0x80)) {
        c = keyboard[scancode];
    }
    else if(mod_keys == MOD_SHIFT && !(scancode & 0x80)) {
        c = keyboard_shift[scancode];
    }
    else if(mod_keys == MOD_ALT && !(scancode & 0x80)) {
        c = keyboard_alt[scancode];
    }
    else if(mod_keys == MOD_CTRL && !(scancode & 0x80)) {
        c = 4;
    }
    else if ( !(scancode & 0x80) ) {
		printf("Not implemented (scancode = %x)\n", scancode);
        c = 0;
	}

    terminal_putchar(c);
    terminal_update_cursor();


mod:
    pic_send_eoi(1);
    return;
}
