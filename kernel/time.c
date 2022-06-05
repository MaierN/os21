#include "time.h"
#include "interrupt.h"
#include "display.h"
#include "util.h"

uint32_t ms_ticks = 0;

static void interrupt_handler(interrupt_registers_t *r) {
    (void)r;
    ms_ticks += 1;
}

void time_init() {
    // https://wiki.osdev.org/PIT
    util_out(0x43, 0b00110110);
    uint16_t val = 1193182 / 1000;
    util_out(0x40, val & 0xff);
    util_out(0x40, (val >> 8) & 0xff);

    interrupt_set_handler(0, interrupt_handler);
}

uint32_t time_ms() {
    return ms_ticks;
}
