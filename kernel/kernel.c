#include <stdint.h>
#include <stddef.h>

#include "display.h"
#include "interrupt.h"
#include "keyboard.h"
#include "taskmgr.h"

#include "../programs/shell.h"

void main() {
    display_init();
    display_clear();
    interrupt_init();
    keyboard_init();
    asm volatile("sti");

    taskmgr_run_program(&program_shell);
    taskmgr_start();
}
