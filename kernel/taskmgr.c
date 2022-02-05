#include "taskmgr.h"

#include "../programs/shell.h"
#include "../programs/help.h"
#include "../programs/snake.h"

taskmgr_program_t *curr_program;

taskmgr_program_t *taskmgr_all_programs[] = {
    &program_help,
    &program_snake,
};

size_t taskmgr_programs_n = 0;

void taskmgr_start() {
    taskmgr_programs_n = sizeof(taskmgr_all_programs) / sizeof(taskmgr_program_t *);

    while (1) {
        curr_program->run();
    }
}
void taskmgr_run_program(taskmgr_program_t *program) {
    curr_program = program;
    curr_program->init();
}
