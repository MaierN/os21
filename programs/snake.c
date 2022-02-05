#include "snake.h"
#include "../kernel/display.h"
#include "../kernel/keyboard.h"
#include "../kernel/taskmgr.h"

#include "shell.h"

void program_snake_init() {
    display_clear();
    display_ln("TODO... (press ESC to quit)");
}

void program_snake_run() {
    char c;
    if (keyboard_read(&c)) {
        if (c == 0x1b) {
            taskmgr_run_program(&program_shell);
        }
    }

}

taskmgr_program_t program_snake = {
    .init = program_snake_init,
    .run = program_snake_run,
    .name = "snake",
    .description = "snake game (play with W A S D, press ESC to quit)",
    .clears_screen = true,
};
