#pragma once

#include <common.h>

uintptr_t get_esp(void);
void print_stack(uintptr_t *stack, int entries);
