section .text
global load_page_directory
global enable_paging

load_page_directory:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8]  ; Load the argument from the stack
    mov cr3, eax        ; Set the page directory base
    mov esp, ebp
    pop ebp
    ret

enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0        ; Read the current value of CR0
    or eax, 0x80000000  ; Set the PG (paging) bit
    mov cr0, eax        ; Write back to CR0
    mov esp, ebp
    pop ebp
    ret
