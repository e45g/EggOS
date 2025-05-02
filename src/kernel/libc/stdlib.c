#include <common.h>

void memset(void *dest, int c, uint32_t n) {
    unsigned char *s = dest;
    for(uint32_t i = 0; i < n; i++) {
        s[i] = c;
    }
}
