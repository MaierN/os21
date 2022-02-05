
; constants
BOOTLOADER_OFFSET equ 0x7C00
KERNEL_OFFSET equ 0x7E00
STACK_OFFSET equ 0x7000
KERNEL_N_SECTORS equ 0x19

bits 16
org BOOTLOADER_OFFSET

jmp bootstart

times 0x03-($-$$) db 0x90
times 0x5a-($-$$) db 0x00

bootstart:
    cli
    cld

    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; save boot drive
    mov [BOOT_DRIVE], dl

    ; setup stack
    mov sp, STACK_OFFSET
    mov bp, STACK_OFFSET

    sti

    mov ah, 0x00
    mov al, 0x03
    int 0x10

    call clearscreen

    ; read kernel sectors
    call loadsectors

    mov bx, boot_done
    call println
    call emptyline

    ; switch to 32bit protected mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp CODE_SEGMENT_OFFSET:init_protected_32bit

    jmp $


null db 0
boot_loading db "loading kernel sectors...", 0
boot_done db "done! running kernel code", 0

sectors_loaded dw 0

loadsectors:
    pusha

  .readsectors:
    mov bx, boot_loading
    call println

    mov ah, 0x02 ; read sectors
    mov al, KERNEL_N_SECTORS
    mov ch, 0x00
    mov cl, 0x02 ; first sector id
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]
    mov bx, 0x0000
    mov es, bx
    mov bx, KERNEL_OFFSET
    int 0x13

    jc .readsectors
    cmp al, KERNEL_N_SECTORS
    jne .readsectors

    mov ax, sectors_loaded
    inc ax
    mov [sectors_loaded], ax

    popa
    ret

clearscreen:
    pusha

    mov ax, 0x100
  .loop:
    call emptyline
    dec ax
    jnz .loop

    popa
    ret

emptyline:
    pusha

    mov bx, null
    call println

    popa
    ret

println:
    pusha

    call printstr
    mov al, 0x0D
    call printchr
    mov al, 0x0A
    call printchr

    popa
    ret

printstr:
    pusha

  .loop:
    mov al, [bx]

    or al, al
    jz .exit

    call printchr

    inc bx
    jmp .loop

  .exit:
    popa
    ret

printchr:
    pusha

    mov ah, 0x0e
    int 0x10

    popa
    ret

BOOT_DRIVE db 0

; Global Descriptor Table
gdt_segments_start:
gdt_null_segment:
    dq 0x00
gdt_code_segment:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data_segment:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_segments_end:

gdt_descriptor:
    dw gdt_segments_end - gdt_segments_start - 1
    dd gdt_segments_start

CODE_SEGMENT_OFFSET equ gdt_code_segment - gdt_segments_start
DATA_SEGMENT_OFFSET equ gdt_data_segment - gdt_segments_start


bits 32
init_protected_32bit:
    mov ax, DATA_SEGMENT_OFFSET
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call CODE_SEGMENT_OFFSET:KERNEL_OFFSET

times 0x1b8-($-$$) db 0x00

db 0x41, 0x41, 0x43, 0x44
db 0x00, 0x00

db 0x80
db 0x01, 0x01, 0x00
db 0x7F
db 59
db 9
db 25
dd 63
dd 6000

times 0x200-2-($-$$) db 0x00
db 0x55, 0xaa
