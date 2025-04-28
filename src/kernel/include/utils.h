#pragma once

#include <common.h>

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

uintptr_t get_esp(void);
void print_stack(uintptr_t *stack, int entries);
