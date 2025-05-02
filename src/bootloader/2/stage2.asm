[org 0x8000]
[bits 16]

KERNEL_SECTORS equ 0x30
KERNEL_ADDRESS equ 0x11000
KERNEL_DEST    equ 0x100000
FRAME_BUFFER_ADDRESS equ 0x9F00

%define ENDL 0x0D, 0x0A

section .data
bootinfo:
    .magic:        dd 0x12345678            ; magic number     - 4 bytes
    .memmap_count: dd 0                     ; memory map count - 4 bytes;
    .memmap_ptr:   dd memory_map_entries    ; memory map ptr   - 4 bytes
    .boot_drive:   db 0                     ; boot drive       - 1 byte
    .padding:      db 0, 0, 0               ; padding          - 3 bytes

section .text
jmp 0x0000:_start

_start:
    mov [bootinfo.boot_drive], dl
    mov sp, 0x7fff
    
    ; welcome message
    mov si, stage_2
    call print

    ; load the kernel
    call load_kernel

    ; activate a20
    call activate_a20

    ; map memory
    call memory_map

    ; vesa
    ; call get_vesa
    ; call enter_vesa

    ; gdt
    lgdt [gdtr]


    ; enter protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp CODE_SEG:protected_mode

    hlt

%include "src/bootloader/2/a20.asm"
%include "src/bootloader/2/load_kernel.asm"
%include "src/bootloader/2/gdt.asm"
%include "src/bootloader/2/memory_map.asm"
%include "src/bootloader/2/vesa.asm"


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
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; move the kernel to 1M
    mov esi, KERNEL_ADDRESS
    mov edi, KERNEL_DEST
    mov ecx, KERNEL_SECTORS
    shl ecx, 9
    add ecx, 3
    shr ecx, 2
    cld
    rep movsd

    ; save bootinfo into ebx
    mov ebx, bootinfo

    call KERNEL_DEST
    cli

loopend:
    hlt
    jmp loopend


section .rodata
stage_2: db 'second_stage :) ', ENDL, 0

section .bss
memory_map_entries: resb 3072
