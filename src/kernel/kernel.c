#include <stdint.h>

int kernel_main() {
    const char *msg = "Hello World!";
    uint16_t *video_memory = (uint16_t *)0xB8000;

    for (int i = 0; msg[i] != '\0'; i++) {
        video_memory[i] = (0x07 << 8) | msg[i];
    }

    return 0;
}

