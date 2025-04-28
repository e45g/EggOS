#include <common.h>

uintptr_t get_esp(void) {
    uintptr_t esp;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp));
    return esp;
}

void print_stack(const uintptr_t *stack, int entries) {
    for(int i = 0; i < entries; i++) {
        printf("%p : %x\n", stack+i, stack[i]);
    }
}

