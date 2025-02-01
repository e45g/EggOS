#pragma once

#include "common.h"

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)


extern void kernel_main();

uintptr_t get_esp();
void print_stack(uintptr_t *stack, int entries);
