#pragma once

#include <common.h>

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)

extern char _kernel_start;
extern char _kernel_end;

extern uintptr_t kernel_end;
extern uintptr_t kernel_start;

extern void kmain();
