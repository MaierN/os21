#include "snake.h"
#include "../kernel/display.h"
#include "../kernel/keyboard.h"
#include "../kernel/taskmgr.h"
#include "../kernel/time.h"

#include "shell.h"

#define TICK_TIME_MS 1000
#define GRID_HEIGHT 10
#define GRID_WIDTH 10
#define GRID_OFFSET_X 5
#define GRID_OFFSET_Y 5
#define SNAKE_MAX_LENGTH (GRID_HEIGHT * GRID_WIDTH)

static uint32_t last_time = 0;
static uint8_t direction = 0;

uint8_t grid[GRID_WIDTH][GRID_HEIGHT];

typedef struct {
    uint8_t x;
    uint8_t y;
} point_t;

struct {
    uint8_t snake_length;
    point_t segments[SNAKE_MAX_LENGTH];
} snake;
point_t apple;

static void render_grid() {
    for (int i = 0; i < GRID_HEIGHT + 2; i++) {
        display_set_char(GRID_OFFSET_X, GRID_OFFSET_Y + i, '#');
        display_set_char(GRID_WIDTH + 2, i, '#');
    }
    for (int i = 0; i < GRID_WIDTH; i++) {
        display_set_char(i, 0, '#');
        display_set_char(i, GRID_HEIGHT + 2, '#');
    }
}

void program_snake_init() {
    display_clear();
    last_time = time_ms();
    direction = 0;

    snake.snake_length = 3;
    snake.segments[0] = (point_t) {.x=4, .y=4};
    snake.segments[1] = (point_t) {.x=3, .y=4};
    snake.segments[2] = (point_t) {.x=2, .y=4};

    for (int i = 0; i < GRID_WIDTH; i++)
        for (int j = 0; j < GRID_HEIGHT; j++)
            grid[i][j] = 0;

    display_set_line("(press ESC to quit)", 0);
    render_grid();
}

void program_snake_run() {
    char c;
    if (keyboard_read(&c)) {
        if (c == 0x1b) {
            taskmgr_run_program(&program_shell);
            return;
        }
        if (c == 'd') direction = 0;
        if (c == 'w') direction = 1;
        if (c == 'a') direction = 2;
        if (c == 's') direction = 3;
    }

    uint32_t curr_time = time_ms();
    uint32_t d_time = curr_time - last_time;

    if (d_time > TICK_TIME_MS) {
        last_time = curr_time;

        
    }
}

taskmgr_program_t program_snake = {
    .init = program_snake_init,
    .run = program_snake_run,
    .name = "snake",
    .description = "snake game (play with W A S D, press ESC to quit)",
    .clears_screen = true,
};
