section .data
global _start
extern kernel_main

section .bss
stack_start:
    resb 16384      ; Reserve 16KB for the stack
stack_end:

section .text.start
_start:
    mov esp, stack_end
    call kernel_main
    mov dword [0xB8000], 0x0A42
    jmp $

times 512 - ($ - $$) db 0
