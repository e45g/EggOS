activate_a20:
    mov     ax, 2403h               ;--- A20-Gate Support ---
    int     15h
    jb      a20_ns                  ;INT 15h is not supported
    cmp     ah, 0
    jnz     a20_ns                  ;INT 15h is not supported

    mov     ax, 2402h                ;--- A20-Gate Status ---
    int     15h
    jb      a20_stat_failed         ;couldn't get status
    cmp     ah, 0
    jnz     a20_stat_failed         ;couldn't get status

    cmp     al, 1
    jz      a20_active           ;A20 is already activated

    mov     ax, 2401h                ;--- A20-Gate Activate ---
    int     15h
    jb      a20_failed              ;couldn't activate the gate
    cmp     ah, 0
    jnz     a20_failed              ;couldn't activate the gate

    mov si, a20_activated_msg
    call print

    ret

a20_ns:
    mov si, a20_ns_msg
    call print
    jmp halt

a20_stat_failed:
    mov si, a20_no_status_msg
    call print
    jmp halt

a20_active:
    mov si, a20_active_msg
    call print
    ret

a20_failed:
    mov si, a20_failed_msg
    call print
    jmp halt

a20_ns_msg: db '>> a20 ns', ENDL, 0
a20_no_status_msg: db '>> a20 stts', ENDL, 0
a20_failed_msg: db '>> a20 fld', ENDL, 0
a20_active_msg: db '>> a20 ?', ENDL, 0
a20_activated_msg: db '>> a20 :)', ENDL, 0
