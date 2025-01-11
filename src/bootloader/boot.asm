[org 0x7C00]
[bits 16]

KERNEL equ 0x7F00

jmp 0x0000:_start

_start:
    ; disable interrupts
    cli

    ; setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x8000
    mov [BOOT_DRIVE], dl; 17*512 allows for a kernel.bin up to 8704 bytes
    mov dh, 17
    mov bx, KERNEL

    ; setup stack
    mov ss, ax
    mov sp, 0x7E00

    ; load kernel
    call load_kernel

    mov si, kernel_msg
    call print

    ; enable A20 line
    call enable_a20

    ; load gdt
    mov ax, 0
    mov ds, ax
    lgdt [gdtr]

    ; switch to protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    nop
    nop
    nop
    nop

    jmp CODE_OFFSET:protected_mode

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

load_kernel:
    push dx

    mov ah, 0x02       ; BIOS read sectors function
    mov al, dh          ; Number of sectors to read
    mov ch, 0x0          ; Cylinder number (0)
    mov cl, 0x02       ; Starting sector (sector 1)
    mov dh, 0x0          ; Head number (0)
    mov bx, KERNEL     ; Address where the kernel will be loaded

    int 0x13
    jc disk_error

    pop dx
    ret

disk_error:
    mov ah, 0x01 ; Get status
    int 0x13
    push ax

    mov si, disk_error_msg
    call print

    pop ax
    mov ah, 0x0E
    add al, '0'              ; Convert to ASCII
    int 0x10

    jmp $

disk_error_msg: db 'Disk error: ', 0

error:
    mov si, error_msg
    call print
    jmp $

gdt_start:
    dq 0

gdt_code:
    dw 0FFFFh           ; limit low
    dw 0                ; base low
    db 0                ; base middle
    db 10011010b        ; access
    db 11001111b        ; granularity
    db 0                ; base high

gdt_data:
    dw 0FFFFh           ; limit low (Same as code)
    dw 0                ; base low
    db 0                ; base middle
    db 10010010b        ; access
    db 11001111b        ; granularity
    db 0                ; base high
end_of_gdt:

gdtr:
    dw end_of_gdt - gdt_start - 1   ; limit (Size of GDT)
    dd gdt_start        ; base of GDT

CODE_OFFSET equ gdt_code - gdt_start
DATA_OFFSET equ gdt_data - gdt_start

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
    ; Set up segments
    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL
    cli

loopend:
    hlt
    jmp loopend

[bits 16]
BOOT_DRIVE: db 0
error_msg: db 'Error :(', 0
kernel_msg: db 'Kernel loaded ', 0
times 510 - ($ - $$) db 0 ; Padding
dw 0xaa55 ; Bootable file
