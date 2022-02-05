#include "interrupt.h"
#include "display.h"
#include "util.h"
#include "asm.h"

#define IDT_N 256

typedef struct {
    uint16_t  limit;
    uint32_t base;
} __attribute__((packed)) idt_record_t;

typedef struct {
    uint16_t low_offset;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

idt_record_t idt_record;
idt_gate_t idt[IDT_N] = {{0}};
interrupt_handler_t interrupt_handlers[16] = {0};

size_t interrupt_error_count = 0;

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(interrupt_registers_t *r) {
    (void)r;
    interrupt_error_count++;
    display_str("received interrupt: ");
    display_hex((uint8_t*)&r->int_no, 4);
    display_str("  -  ");
    display_ln(exception_messages[r->int_no]);
    display_hex((uint8_t*)&r, 32);
    //while (1) {}
}

void irq_handler(interrupt_registers_t *r) {
    if (interrupt_handlers[r->err_code] != 0) {
        interrupt_handlers[r->err_code](r);
        if (r->err_code != 1) {
            display_ln("");
            display_str("calling interrupt handler");
            display_ln("");
        }
    }

    if (r->int_no >= 40) {
        util_out(0xA0, 0x20);
    }
    util_out(0x20, 0x20);
}

void interrupt_set_handler(size_t id, interrupt_handler_t handler) {
    interrupt_handlers[id] = handler;
}

void interrupt_set_gate(int n, uint32_t handler) {
    idt[n].low_offset = UTIL_32_LOW_16(handler);
    idt[n].selector = 0x08; // code segment in GDT
    idt[n].always0 = 0;
    // 0x8E = 1  00 0 1  110
    //        P DPL 0 D Type
    idt[n].flags = 0x8E;
    idt[n].high_offset = UTIL_32_HIGH_16(handler);
}

void interrupt_init() {
    interrupt_set_gate(0, (uint32_t) isr0);
    interrupt_set_gate(1, (uint32_t) isr1);
    interrupt_set_gate(2, (uint32_t) isr2);
    interrupt_set_gate(3, (uint32_t) isr3);
    interrupt_set_gate(4, (uint32_t) isr4);
    interrupt_set_gate(5, (uint32_t) isr5);
    interrupt_set_gate(6, (uint32_t) isr6);
    interrupt_set_gate(7, (uint32_t) isr7);
    interrupt_set_gate(8, (uint32_t) isr8);
    interrupt_set_gate(9, (uint32_t) isr9);
    interrupt_set_gate(10, (uint32_t) isr10);
    interrupt_set_gate(11, (uint32_t) isr11);
    interrupt_set_gate(12, (uint32_t) isr12);
    interrupt_set_gate(13, (uint32_t) isr13);
    interrupt_set_gate(14, (uint32_t) isr14);
    interrupt_set_gate(15, (uint32_t) isr15);
    interrupt_set_gate(16, (uint32_t) isr16);
    interrupt_set_gate(17, (uint32_t) isr17);
    interrupt_set_gate(18, (uint32_t) isr18);
    interrupt_set_gate(19, (uint32_t) isr19);
    interrupt_set_gate(20, (uint32_t) isr20);
    interrupt_set_gate(21, (uint32_t) isr21);
    interrupt_set_gate(22, (uint32_t) isr22);
    interrupt_set_gate(23, (uint32_t) isr23);
    interrupt_set_gate(24, (uint32_t) isr24);
    interrupt_set_gate(25, (uint32_t) isr25);
    interrupt_set_gate(26, (uint32_t) isr26);
    interrupt_set_gate(27, (uint32_t) isr27);
    interrupt_set_gate(28, (uint32_t) isr28);
    interrupt_set_gate(29, (uint32_t) isr29);
    interrupt_set_gate(30, (uint32_t) isr30);
    interrupt_set_gate(31, (uint32_t) isr31);

    // ICW1
    util_out(0x20, 0x11);
    util_out(0xA0, 0x11);
    // ICW2
    util_out(0x21, 0x20);
    util_out(0xA1, 0x28);
    // ICW3
    util_out(0x21, 0x04);
    util_out(0xA1, 0x02);
    // ICW4
    util_out(0x21, 0x01);
    util_out(0xA1, 0x01);
    // OCW1
    util_out(0x21, 0x0);
    util_out(0xA1, 0x0);

    interrupt_set_gate(32, (uint32_t)irq0);
    interrupt_set_gate(33, (uint32_t)irq1);
    interrupt_set_gate(34, (uint32_t)irq2);
    interrupt_set_gate(35, (uint32_t)irq3);
    interrupt_set_gate(36, (uint32_t)irq4);
    interrupt_set_gate(37, (uint32_t)irq5);
    interrupt_set_gate(38, (uint32_t)irq6);
    interrupt_set_gate(39, (uint32_t)irq7);
    interrupt_set_gate(40, (uint32_t)irq8);
    interrupt_set_gate(41, (uint32_t)irq9);
    interrupt_set_gate(42, (uint32_t)irq10);
    interrupt_set_gate(43, (uint32_t)irq11);
    interrupt_set_gate(44, (uint32_t)irq12);
    interrupt_set_gate(45, (uint32_t)irq13);
    interrupt_set_gate(46, (uint32_t)irq14);
    interrupt_set_gate(47, (uint32_t)irq15);

    for (size_t i = 48; i < IDT_N; i++) {
        interrupt_set_gate(i, (uint32_t)irq15);
    }

    idt_record.base = (uint32_t)&idt;
    idt_record.limit = IDT_N * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_record));
}
