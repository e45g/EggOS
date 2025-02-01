#include <common.h>
#include <tty.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void putchar(char c) {
    terminal_putchar(c);
}

void printf(const char *fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);

    while(*fmt) {
        if(*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '%':
                    putchar('%');
                    break;

                case 's': {
                        const char *s = va_arg(vargs, const char *);
                        terminal_writestring(s);
                    }
                    break;

                case 'c': {
                        int c = va_arg(vargs, int);
                        putchar(c);
                    }
                    break;

                case 'd': {
                        int value = va_arg(vargs, int);
                        if(value < 0) {
                            value = -value;
                            putchar('-');
                        }

                        int divisor = 1;
                        while(value / divisor > 9) divisor *= 10;

                        while(value > 0) {
                            putchar('0' + value / divisor);
                            value %= divisor;
                            divisor /= 10;
                        }
                    }
                    break;

                case 'x': {
                        unsigned int value = va_arg(vargs, unsigned int);

                        putchar('0');
                        putchar('x');
                        for(int i = 7; i >= 0; i--) {
                            int nibble = (value >> (i * 4)) & 0xF;
                            putchar("0123456789abcdef"[nibble]);
                        }
                    }
                    break;
                case 'p': {
                        void *ptr = va_arg(vargs, void *);
                        unsigned long value = (unsigned long)ptr;

                        putchar('0');
                        putchar('x');
                        for (int i = 7; i >= 0; i--) {
                            int nibble = (value >> (i * 4)) & 0xF;
                            putchar("0123456789abcdef"[nibble]);
                        }
                    }
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
