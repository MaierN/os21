#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
    uint32_t int_no, err_code; // pushed manually
    uint32_t eip, cs, eflags, useresp, ss; // pushed by CPU
} __attribute__((packed)) interrupt_registers_t;

typedef void (*interrupt_handler_t)(interrupt_registers_t *r);

extern size_t interrupt_error_count;

void interrupt_init();
void interrupt_set_handler(size_t id, interrupt_handler_t handler);

#endif