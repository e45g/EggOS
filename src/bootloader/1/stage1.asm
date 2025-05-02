[org 0x7C00]
[bits 16]

%define ENDL 0x0D, 0x0A

jmp 0x0000:_start

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7FF0

    mov [boot_drive], dl;

    mov si, welcome_msg
    call print

    mov bx, 0x8000
    mov es, ax
    mov ah, 0x02
    mov al, 0x10        ; sectors
    mov ch, 0           ; cylinder
    mov cl, 2           ; sector 2
    mov dh, 0           ; head
    mov dl, [boot_drive]        ; first hard disk
    int 0x13
    jc disk_error

    jmp 0x0000:0x8000
    hlt

disk_error:
    mov si, disk_error_msg
    call print
    jmp halt

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


boot_drive: db 0
disk_error_msg: db 'Disk error, stage 1 :(';
welcome_msg: db 'Wlcm :)', ENDL, 0

times 510 - ($ - $$) db 0
dw 0xAA55
