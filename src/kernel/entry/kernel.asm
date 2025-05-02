section .data
global _start
extern kmain

section .bss
align 16
stack_start:
    resb 16384      ; Reserve 16KB for the stack
stack_end:

section .text.start
_start:
    mov esp, stack_end

    ; zero fill bss
;    extern bss_start
;    extern bss_end
;    mov edi, bss_start
;    mov ecx, bss_end
;    sub ecx, edi
;    shr ecx, 2            ; divide size by 4 (bytes -> dwords)
;    xor eax, eax
;    rep stosd

    call kmain
    jmp $
