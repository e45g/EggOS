MODE equ 0x11A

enter_vesa:
    mov ax, 0x4F02
    mov bx, MODE
    int 0x10
    cmp ah, 0x1
    je vesa_failed

    cmp ah, 0x0
    jmp vesa_done

vesa_failed:
    mov si, vesa_failed_msg
    call print
    jmp halt

vesa_done:
    mov si, vesa_success_msg
    call print
    jmp .get_info

.get_info:
    mov ax, 0x4F01
    mov cx, MODE
    mov di, vbe_mode_info
    int 0x10

    cmp al, 0x4F
    jne vesa_failed

    lea bx, [vbe_mode_info + 0x28]
    mov di, bx
    mov ax, [di]
    mov dx, [di+2]

    mov [FRAME_BUFFER_ADDRESS], ax
    mov [FRAME_BUFFER_ADDRESS+2], dx

    ret


vesa_failed_msg: db 'vesa :('
vesa_success_msg: db 'vesa :)'

; vbe_info: times 256 db 0
; mode_list_ptr  dw 0

vbe_mode_info: times 256 db 0

