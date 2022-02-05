#ifndef TASKMGR_H
#define TASKMGR_H

#include "stddef.h"
#include "stdbool.h"

#define TASKMGR_PROGRAM_NAME_LENGTH 10
#define TASKMGR_PROGRAM_DESCRIPTION_LENGTH 60

typedef void (*taskmgr_init_t)();
typedef void (*taskmgr_run_t)();

typedef struct {
    taskmgr_init_t init;
    taskmgr_run_t run;
    char name[TASKMGR_PROGRAM_NAME_LENGTH];
    char description[TASKMGR_PROGRAM_DESCRIPTION_LENGTH];
    bool clears_screen;
} taskmgr_program_t;

extern taskmgr_program_t *taskmgr_all_programs[];
extern size_t taskmgr_programs_n;

void taskmgr_start();
void taskmgr_run_program(taskmgr_program_t *program);

#endif