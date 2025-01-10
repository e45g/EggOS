[org 0x7C00]
[bits 16]

CODE_OFFSET equ 0x08
DATA_OFFSET equ 0x10

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDR equ 0x100000

jmp 0x0000:start

start:
    ; disable interrupts
    cli

    ; setup segments
    mov ax, 0
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7C00

    ; load gdt
    lgdt [gdt_descriptor]

    ; load kernel
    mov bx, KERNEL_LOAD_SEG
    mov ah, 0x02
    mov al, 0x20
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x0
    mov dl, 0x80

    int 0x13

    jc error

    mov si, kernel_msg
    call print

    ; switch to protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp CODE_OFFSET:protected_mode


error:
    mov si, error_msg
    call print
    hlt

gdt_start:
    ; Null descriptor
    dq 0x0

    ; Code segment
    dw 0xFFFF ; Limit
    dw 0x0000 ; Base
    db 0x00 ; Base
    db 0x9A ; Access byte
    db 0xCF ; Flags and Limit
    db 0x00 ; Base

    ; Data segment
    dw 0xFFFF ; Limit
    dw 0x0000 ; Base
    db 0x00 ; Base
    db 0x92 ; Access byte
    db 0xCF ; Flags and Limit
    db 0x00 ; Base

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; Params:
;    - ds:si points to string
print:
    push si
    push ax

.loop:
    lodsb               ; load next character into al
    cmp al, 0
    je .done

    mov ah, 0x0e
    mov bh, 0
    int 0x10

    jmp .loop

.done:
    pop ax
    pop si
    ret

[bits 32]
protected_mode:
    ; A20 line
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ;execute kernel
    mov ax,10h
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp CODE_OFFSET:KERNEL_START_ADDR



error_msg: db 'Error :(', 0
kernel_msg: db 'Kernel loaded ', 0
times 510 - ($ - $$) db 0 ; Padding
dw 0xaa55 ; Bootable file
