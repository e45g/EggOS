load_kernel:
    push dx
    mov dh, KERNEL_SECTORS

    mov ah, 0x02 ; BIOS mode
    mov al, dh   ; Sector count
    mov ch, 0x00 ; Cylinder
    mov cl, 0x02 ; Starting sector
    mov dh, 0x00 ; Head number
    mov bx, KERNEL_ADDRESS >> 4
    mov es, bx
    mov bx, 0x0

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


disk_error_msg: db 'drive :(', ENDL, 0
disk_success_msg: db 'drive :)', ENDL, 0
