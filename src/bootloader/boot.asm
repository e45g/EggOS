[org 0x7C00]
[bits 16]

KERNEL_ADDRESS equ 0x10000
MEMORY_MAP_ADDRESS equ 0x8000

%define ENDL 0x0D, 0x0A

jmp 0x0000:_start

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00

    mov [boot_drive], dl;
    mov dh, 0x20
    mov bx, 0x1000

    ; welcome message
    mov si, welcome_msg
    call print

    ; load the kernel
    call load_kernel

    ; activate a20
    call activate_a20

    ; map memory
    call memory_map

    ; gdt
    lgdt [gdtr]

    ; enter protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp CODE_OFFSET:protected_mode

    hlt

%include "src/bootloader/a20.asm"
%include "src/bootloader/load_kernel.asm"
%include "src/bootloader/gdt.asm"
%include "src/bootloader/memory_map.asm"


print:
    push ax
    push si

    mov ah, 0x0E
    mov bh, 0x0
    jmp .loop

.loop:
    lodsb
    or al, al
    je .done
    int 0x10
    jmp .loop

.done:
    pop si
    pop ax
    ret

halt:
    hlt
    jmp halt


[bits 32]
protected_mode:

    ; setup segments and stack
    mov ax, CODE_OFFSET
    mov cs, ax

    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call KERNEL_ADDRESS
    mov dword [0xB8000], 0x0A41
    cli

loopend:
    hlt
    jmp loopend


[bits 16]
boot_drive: db 0
welcome_msg: db 'Wlcm :)', ENDL, 0

times 510 - ($ - $$) db 0
dw 0xAA55
