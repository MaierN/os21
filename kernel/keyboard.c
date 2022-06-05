#include "keyboard.h"
#include "interrupt.h"
#include "display.h"
#include "util.h"

const char scancodes[] = {'?', 0x1b, '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', 0x08, '?', 'q', 'w', 'e', 'r', 't', 'z',
                         'u', 'i', 'o', 'p', '[', ']', '\n', '?', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'y', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

char last_letter = 0;

static void interrupt_handler(interrupt_registers_t *r) {
    (void)r;

    uint8_t scancode = util_in(0x60);
    if (scancode < sizeof(scancodes) / sizeof(char)) {
        last_letter = scancodes[scancode];
    }
}

void keyboard_init() {
    interrupt_set_handler(1, interrupt_handler);
}

bool keyboard_read(char *letter) {
    if (last_letter == 0) return false;
    *letter = last_letter;
    last_letter = 0;
    return true;
}
