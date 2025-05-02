memory_map:
    xor ax, ax
    mov es, ax
    mov di, memory_map_entries
 
    xor bp, bp

    xor ebx, ebx
    mov edx, 0x534D4150

memory_map_entry:
    mov eax, 0xe820
    mov dword [es:di+20], 0x1
    mov ecx, 24
    int 0x15

    jc map_failed

    mov edx, 0x0534D4150 ; possibly trashed

    cmp eax, edx
    jne map_failed

    jcxz next_entry

    cmp cl, 20
	jbe short save_entry

    jmp next_entry


save_entry:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz next_entry		; length is 0, skip entry
	inc bp
	add di, 24

next_entry:
    test ebx, ebx
    jne memory_map_entry

    mov [bootinfo.memmap_count], bp
    ret


map_failed:
    mov si, memory_map_msg
    call print
    hlt

memory_map_msg: db 'memmap :(', ENDL, 0
