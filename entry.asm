
bits 32
[extern main]
global _start
_start:
    call main
    mov al, 0x45
    mov [0xb8000], al
    mov al, 0x4e
    mov [0xb8002], al
    mov al, 0x44
    mov [0xb8004], al
    jmp $

[extern isr_handler]
[extern irq_handler]

isrcommon:
    ; push general purpose registers
    pusha

    ; push data segment selector
    mov ax, ds
    push eax

    ; use kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; hand over stack to C function
    push esp
    ; and call it
    call isr_handler
    ; pop stack pointer again
    pop eax

    ; restore original segment pointers segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; restore registers
    popa

    ; remove int_no and err_code from stack
    add esp, 8

    sti

    ; pops cs, eip, eflags, ss, and esp
    ; https://www.felixcloutier.com/x86/iret:iretd
    iret

irqcommon:
    ; push general purpose registers
    pusha

    ; push data segment selector
    mov ax, ds
    push eax

    ; use kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; hand over stack to C function
    push esp
    ; and call it
    call irq_handler
    ; pop stack pointer again
    pop ebx

    ; restore original segment pointers segment
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    ; restore registers
    popa

    ; remove int_no and err_code from stack
    add esp, 8

    sti

    ; pops cs, eip, eflags, ss, and esp
    ; https://www.felixcloutier.com/x86/iret:iretd
    iret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; 0: Divide By Zero Exception
isr0:
    push byte 0
    push byte 0
    jmp isrcommon

; 1: Debug Exception
isr1:
    push byte 0
    push byte 1
    jmp isrcommon

; 2: Non Maskable Interrupt Exception
isr2:
    push byte 0
    push byte 2
    jmp isrcommon

; 3: Int 3 Exception
isr3:
    push byte 0
    push byte 3
    jmp isrcommon

; 4: INTO Exception
isr4:
    push byte 0
    push byte 4
    jmp isrcommon

; 5: Out of Bounds Exception
isr5:
    push byte 0
    push byte 5
    jmp isrcommon

; 6: Invalid Opcode Exception
isr6:
    push byte 0
    push byte 6
    jmp isrcommon

; 7: Coprocessor Not Available Exception
isr7:
    push byte 0
    push byte 7
    jmp isrcommon

; 8: Double Fault Exception (With Error Code!)
isr8:
    push byte 8
    jmp isrcommon

; 9: Coprocessor Segment Overrun Exception
isr9:
    push byte 0
    push byte 9
    jmp isrcommon

; 10: Bad TSS Exception (With Error Code!)
isr10:
    push byte 10
    jmp isrcommon

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    push byte 11
    jmp isrcommon

; 12: Stack Fault Exception (With Error Code!)
isr12:
    push byte 12
    jmp isrcommon

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    push byte 13
    jmp isrcommon

; 14: Page Fault Exception (With Error Code!)
isr14:
    push byte 14
    jmp isrcommon

; 15: Reserved Exception
isr15:
    push byte 0
    push byte 15
    jmp isrcommon

; 16: Floating Point Exception
isr16:
    push byte 0
    push byte 16
    jmp isrcommon

; 17: Alignment Check Exception
isr17:
    push byte 0
    push byte 17
    jmp isrcommon

; 18: Machine Check Exception
isr18:
    push byte 0
    push byte 18
    jmp isrcommon

; 19: Reserved
isr19:
    push byte 0
    push byte 19
    jmp isrcommon

; 20: Reserved
isr20:
    push byte 0
    push byte 20
    jmp isrcommon

; 21: Reserved
isr21:
    push byte 0
    push byte 21
    jmp isrcommon

; 22: Reserved
isr22:
    push byte 0
    push byte 22
    jmp isrcommon

; 23: Reserved
isr23:
    push byte 0
    push byte 23
    jmp isrcommon

; 24: Reserved
isr24:
    push byte 0
    push byte 24
    jmp isrcommon

; 25: Reserved
isr25:
    push byte 0
    push byte 25
    jmp isrcommon

; 26: Reserved
isr26:
    push byte 0
    push byte 26
    jmp isrcommon

; 27: Reserved
isr27:
    push byte 0
    push byte 27
    jmp isrcommon

; 28: Reserved
isr28:
    push byte 0
    push byte 28
    jmp isrcommon

; 29: Reserved
isr29:
    push byte 0
    push byte 29
    jmp isrcommon

; 30: Reserved
isr30:
    push byte 0
    push byte 30
    jmp isrcommon

; 31: Reserved
isr31:
    push byte 0
    push byte 31
    jmp isrcommon

; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; IRQ handlers
irq0:
    push byte 0
    push byte 32
    jmp irqcommon

irq1:
    push byte 1
    push byte 33
    jmp irqcommon

irq2:
    push byte 2
    push byte 34
    jmp irqcommon

irq3:
    push byte 3
    push byte 35
    jmp irqcommon

irq4:
    push byte 4
    push byte 36
    jmp irqcommon

irq5:
    push byte 5
    push byte 37
    jmp irqcommon

irq6:
    push byte 6
    push byte 38
    jmp irqcommon

irq7:
    push byte 7
    push byte 39
    jmp irqcommon

irq8:
    push byte 8
    push byte 40
    jmp irqcommon

irq9:
    push byte 9
    push byte 41
    jmp irqcommon

irq10:
    push byte 10
    push byte 42
    jmp irqcommon

irq11:
    push byte 11
    push byte 43
    jmp irqcommon

irq12:
    push byte 12
    push byte 44
    jmp irqcommon

irq13:
    push byte 13
    push byte 45
    jmp irqcommon

irq14:
    push byte 14
    push byte 46
    jmp irqcommon

irq15:
    push byte 15
    push byte 47
    jmp irqcommon
