section .data
global _start
extern kernel_main

section .text.start
_start:
    call kernel_main
    mov dword [0xB8000], 0x0A42
    jmp $

times 512 - ($ - $$) db 0
