#include "shell.h"
#include "../kernel/display.h"
#include "../kernel/keyboard.h"
#include "../kernel/taskmgr.h"
#include "../kernel/util.h"

#include "help.h"

#define COMMAND_MAX_LENGTH 50

char curr_line[DISPLAY_WIDTH];
char *curr_command;
size_t curr_command_i;

bool full_init = true;

void program_shell_init() {
    curr_line[0] = '>';
    curr_line[1] = ' ';
    curr_line[2] = '_';
    curr_line[3] = 0;
    curr_command = curr_line + 2;
    curr_command_i = 0;

    if (full_init) {
        display_clear();

        // https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=OS%2015
        display_ln("  /$$$$$$   /$$$$$$          /$$   /$$$$$$$ ");
        display_ln(" /$$__  $$ /$$__  $$       /$$$$  | $$____/ ");
        display_ln("| $$  \\ $$| $$  \\__/      |_  $$  | $$      ");
        display_ln("| $$  | $$|  $$$$$$         | $$  | $$$$$$$ ");
        display_ln("| $$  | $$ \\____  $$        | $$  |_____  $$");
        display_ln("| $$  | $$ /$$  \\ $$        | $$   /$$  \\ $$");
        display_ln("|  $$$$$$/|  $$$$$$/       /$$$$$$|  $$$$$$/");
        display_ln(" \\______/  \\______/       |______/ \\______/ ");

        display_ln("\n\n\nWelcome to the shell!\n\ntype \"help\" for a list of commands");
    }
    full_init = false;
}

void program_shell_run() {
    char c;
    if (keyboard_read(&c)) {
        if (c == '\n') {
            curr_command[curr_command_i] = 0;
            display_set_line(curr_line, DISPLAY_HEIGHT - 1);
            display_ln("");

            for (size_t i = 0; i < taskmgr_programs_n; i++) {
                if (util_str_cmp(taskmgr_all_programs[i]->name, curr_command)) {
                    full_init = taskmgr_all_programs[i]->clears_screen;
                    taskmgr_run_program(taskmgr_all_programs[i]);
                    return;
                }
            }

            if (util_str_len(curr_command) > 0) display_ln("unknown command");
            program_shell_init();

        } else if (c == 0x08) {
            if (curr_command_i > 0) {
                curr_command[curr_command_i--] = 0;
                curr_command[curr_command_i] = '_';
            }
        } else {
            if (curr_command_i < COMMAND_MAX_LENGTH) {
                curr_command[curr_command_i++] = c;
                curr_command[curr_command_i] = '_';
                curr_command[curr_command_i + 1] = 0;
            }
        }
    }

    display_set_line(curr_line, DISPLAY_HEIGHT - 1);
}

taskmgr_program_t program_shell = {
    .init = program_shell_init,
    .run = program_shell_run,
    .name = "shell",
    .description = "shell",
    .clears_screen = false,
};
