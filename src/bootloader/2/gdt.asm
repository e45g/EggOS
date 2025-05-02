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

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
