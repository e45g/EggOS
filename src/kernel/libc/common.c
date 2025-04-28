#include <common.h>
#include <tty.h>

void putchar(char c) {
    if (c == '\n') {
        terminal_putchar('\r');
        terminal_putchar('\n');
    } else {
        terminal_putchar(c);
    }
}

static void print_unsigned(unsigned int value) {
    if (value == 0) {
        putchar('0');
        return;
    }
    unsigned int div = 1;
    while (value / div > 9) div *= 10;
    while (div) {
        putchar('0' + (value / div));
        value %= div;
        div /= 10;
    }
}

static void print_signed(int value) {
    if (value < 0) {
        putchar('-');
        if (value == INT_MIN) {
            unsigned int u = (unsigned int)(-(value + 1)) + 1;
            print_unsigned(u);
            return;
        }
        value = -value;
    }
    print_unsigned((unsigned int)value);
}

static void print_hex(unsigned long value, int digits) {
    bool started = false;
    for (int i = digits - 1; i >= 0; i--) {
        unsigned long nibble = (value >> (i * 4)) & 0xF;
        if (nibble || started || i == 0) {
            putchar("0123456789abcdef"[nibble]);
            started = true;
        }
    }
}

void printf(const char *fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);

    while(*fmt) {
        if(*fmt == '%') {
            fmt++;

            bool long_mod = 0;
            if(*fmt == 'l') {
                long_mod = 1;
                fmt++;
            }

            switch (*fmt) {
            case '%': {
                putchar('%');
                break;
            }

            case 's': {
                const char *s = va_arg(vargs, const char *);
                terminal_writestring(s);
                break;
            }

            case 'c': {
                int c = va_arg(vargs, int);
                putchar(c);
                break;
            }

            case 'd': {
                if (long_mod) {
                    long value = va_arg(vargs, long);
                    print_signed(value);
                } else {
                    int value = va_arg(vargs, int);
                    print_signed((long)value);
                }
                break;
            }

            case 'u': {
                if (long_mod) {
                    unsigned long value = va_arg(vargs, unsigned long);
                    print_unsigned(value);
                } else {
                    unsigned int value = va_arg(vargs, unsigned int);
                    print_unsigned((unsigned long)value);
                }
                break;
            }

            case 'x': {
                if (long_mod) {
                    unsigned long value = va_arg(vargs, unsigned long);
                    print_hex(value, sizeof(unsigned long) * 2);
                } else {
                    unsigned int value = va_arg(vargs, unsigned int);
                    print_hex((unsigned long)value, sizeof(unsigned int) * 2);
                }
                break;
            }

            case 'p': {
                void *ptr = va_arg(vargs, void *);
                unsigned long value = (unsigned long)ptr;

                putchar('0');
                putchar('x');
                for (int i = 7; i >= 0; i--) {
                    int nibble = (value >> (i * 4)) & 0xF;
                    putchar("0123456789abcdef"[nibble]);
                }
                break;
            }

            default:
                putchar('%');
                putchar(*fmt);
                break;
            }
        } else {
            terminal_putchar(*fmt);
        }
        fmt++;
    }
    va_end(vargs);
    terminal_update_cursor();
}
