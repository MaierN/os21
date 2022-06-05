#include "help.h"
#include "../kernel/display.h"
#include "../kernel/taskmgr.h"
#include "../kernel/interrupt.h"

#include "shell.h"

char help_line[DISPLAY_WIDTH];

void program_help_init() {
    for (size_t i = 0; i < taskmgr_programs_n; i++) {
        size_t j = 0;
        for (; j < TASKMGR_PROGRAM_NAME_LENGTH && taskmgr_all_programs[i]->name[j] != 0; j++) {

        }

        for (size_t j = 0; j < DISPLAY_WIDTH; j++) help_line[j] = ' ';
        size_t line_i = 1;

        char *name = taskmgr_all_programs[i]->name;
        while (*name) {
            help_line[line_i++] = *name;
            name++;
        }

        line_i = TASKMGR_PROGRAM_NAME_LENGTH + 2;
        char *description = taskmgr_all_programs[i]->description;
        while (*description) {
            help_line[line_i++] = *description;
            description++;
        }

        display_ln(help_line);
    }
    display_ln("");
    taskmgr_run_program(&program_shell);
}

void program_help_run() {
}

taskmgr_program_t program_help = {
    .init = program_help_init,
    .run = program_help_run,
    .name = "help",
    .description = "displays this help message",
    .clears_screen = false,
};
