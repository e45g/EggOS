[org 0x7C00]
[bits 16]

KERNEL_ADDRESS equ 0x7F00

%define ENDL 0x0aD, 0x0A

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
    mov bx, KERNEL_ADDRESS

    ; welcome message
    mov si, welcome_msg
    call print

    ; load the kernel
    call load_kernel

    ; activate a20
    call activate_a20

    ; gdt
    lgdt [gdtr]

    ; enter protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp CODE_OFFSET:protected_mode

    hlt

%include "src/bootloader/a20.asm"

load_kernel:
    push dx

    mov ah, 0x02 ; BIOS mode
    mov al, dh ; Sector count
    mov ch, 0x00 ; Cylinder
    mov cl, 0x02 ; Starting sector
    mov dh, 0x00 ; Head number
    mov bx, KERNEL_ADDRESS

    int 0x13

    jc disk_error

    mov si, disk_success_msg
    call print

    pop dx
    ret

disk_error:
    mov si, disk_error_msg
    call print

    jmp halt

gdt_start:
    dq 0x0 ; Null

gdt_code:
    dw 0xFFFF    ; Limit
    dw 0x0000    ; Base
    db 0x00      ; Base
    db 10011010b ; Access byte
    db 11001111b ; Flags + Limit
    db 0x0       ; Base

gdt_data:
    dw 0xFFFF    ; Limit
    dw 0x0000    ; Base
    db 0x00      ; Base
    db 10010010b ; Access byte
    db 11001111b ; Flags + Limit
    db 0x0       ; Base

gdt_end:

gdtr:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_OFFSET equ gdt_code - gdt_start
DATA_OFFSET equ gdt_data - gdt_start


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
    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0xA000

    call KERNEL_ADDRESS
    mov dword [0xB8000], 0x0A41
    cli

loopend:
    hlt
    jmp loopend


[bits 16]
boot_drive: db 0
welcome_msg: db 'Welcome :)', ENDL, 0
disk_error_msg: db 'Drive Error :(', ENDL, 0
disk_success_msg: db 'Kernel loaded :)', ENDL, 0

times 510 - ($ - $$) db 0
dw 0xAA55
