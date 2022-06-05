#include "snake.h"
#include "../kernel/display.h"
#include "../kernel/keyboard.h"
#include "../kernel/taskmgr.h"
#include "../kernel/time.h"
#include "../kernel/color.h"
#include "../kernel/rand.h"
#include "../kernel/string.h"

#include "shell.h"

#define TICK_TIME_MS 250
#define GRID_HEIGHT 20
#define GRID_WIDTH 20
#define GRID_OFFSET_X 2
#define GRID_OFFSET_Y 3
#define SNAKE_MAX_LENGTH (GRID_HEIGHT * GRID_WIDTH)

#define COLOR_BACKGROUND COLOR_LIGHT_GREY
#define COLOR_WALL COLOR_DARK_GREY
#define COLOR_SNAKE COLOR_GREEN
#define COLOR_APPLE COLOR_RED

#define CHAR_WALL '#'
#define CHAR_SNAKE 'x'
#define CHAR_APPLE 'o'

typedef struct {
    int8_t x;
    int8_t y;
} point_t;

static uint32_t last_time;
static int8_t direction;
static int8_t last_direction;
static bool game_over;
static bool started;
struct {
    uint8_t snake_length;
    point_t segments[SNAKE_MAX_LENGTH];
} snake;
point_t apple;

static bool eq_point(point_t a, point_t b) {
    return a.x == b.x && a.y == b.y;
}

static void set_double_char(size_t col, size_t row, char c, uint8_t color_char, uint8_t color_bg, bool end_of_line) {
    display_set_char(col * 2, row, c, color_char, color_bg);
    display_set_char(col * 2 + 1, row, ' ', color_char, end_of_line ? COLOR_BLACK : color_bg);
}

static void render_grid() {
    static uint8_t grid_char[GRID_WIDTH][GRID_HEIGHT];
    static uint8_t grid_color[GRID_WIDTH][GRID_HEIGHT];

    for (int i = 0; i < GRID_WIDTH + 2; i++) {
        set_double_char(GRID_OFFSET_X - 1 + i, GRID_OFFSET_Y - 1, CHAR_WALL, COLOR_WALL, COLOR_BACKGROUND, false);
        set_double_char(GRID_OFFSET_X - 1 + i, GRID_OFFSET_Y + GRID_HEIGHT, CHAR_WALL, COLOR_WALL, COLOR_BACKGROUND, false);
    }
    for (int i = 0; i < GRID_HEIGHT + 2; i++) {
        set_double_char(GRID_OFFSET_X - 1, GRID_OFFSET_Y - 1 + i, CHAR_WALL, COLOR_WALL, COLOR_BACKGROUND, false);
        set_double_char(GRID_OFFSET_X + GRID_WIDTH, GRID_OFFSET_Y - 1 + i, CHAR_WALL, COLOR_WALL, COLOR_BACKGROUND, true);
    }

    for (int col = 0; col < GRID_WIDTH; col++) {
        for (int row = 0; row < GRID_HEIGHT; row++) {
            grid_char[col][row] = ' ';
            grid_color[col][row]  = COLOR_BACKGROUND;
        }
    }
    grid_char[apple.x][apple.y] = CHAR_APPLE;
    grid_color[apple.x][apple.y] = COLOR_APPLE;
    for (int i = 0; i < snake.snake_length; i++) {
        grid_char[snake.segments[i].x][snake.segments[i].y] = CHAR_SNAKE;
        grid_color[snake.segments[i].x][snake.segments[i].y] = COLOR_SNAKE;
    }
    for (int col = 0; col < GRID_WIDTH; col++) {
        for (int row = 0; row < GRID_HEIGHT; row++) {
            set_double_char(GRID_OFFSET_X + col, GRID_OFFSET_Y + row, grid_char[col][row], grid_color[col][row], COLOR_BACKGROUND, false);
        }
    }
}

static bool random_apple() {
    apple.x = rand() % GRID_WIDTH;
    apple.y = rand() % GRID_HEIGHT;
    for (int i = 0; i < snake.snake_length; i++) if (eq_point(apple, snake.segments[i])) return false;
    return true;
}

static void centered_message(char *msg, uint8_t color, int8_t y_offset) {
    for (size_t i = 0; i < strlen(msg); i++) display_set_char(GRID_OFFSET_X + (GRID_WIDTH * 2) / 2 - strlen(msg) / 2 + i, GRID_OFFSET_Y + GRID_HEIGHT / 2 + y_offset, msg[i], color, COLOR_BACKGROUND);
}

void program_snake_init() {
    display_clear();
    srand(time_ms());

    last_time = time_ms();
    direction = 0;
    last_direction = 0;
    game_over = false;
    started = false;

    snake.snake_length = 3;
    snake.segments[0] = (point_t) {.x=4, .y=4};
    snake.segments[1] = (point_t) {.x=3, .y=4};
    snake.segments[2] = (point_t) {.x=2, .y=4};

    while (!random_apple());

    display_set_line("(press ESC to quit)", 0);
    render_grid();

    centered_message("Play with W A S D", COLOR_WHITE, 0);
    centered_message("(press any key to start)", COLOR_WHITE, 2);
}

void program_snake_run() {
    char c;
    if (keyboard_read(&c)) {
        if (c == 0x1b) {
            taskmgr_run_program(&program_shell);
            return;
        }
        if (c == 'd' && last_direction != 2) direction = 0;
        if (c == 'w' && last_direction != 3) direction = 1;
        if (c == 'a' && last_direction != 0) direction = 2;
        if (c == 's' && last_direction != 1) direction = 3;

        started = true;
    }

    uint32_t curr_time = time_ms();
    uint32_t d_time = curr_time - last_time;

    if (d_time > TICK_TIME_MS && !game_over && started) {
        last_time = curr_time;

        for (int i = snake.snake_length - 1; i > 0; i--) {
            snake.segments[i] = snake.segments[i - 1];
        }
        if (direction == 0) snake.segments[0].x += 1;
        if (direction == 1) snake.segments[0].y -= 1;
        if (direction == 2) snake.segments[0].x -= 1;
        if (direction == 3) snake.segments[0].y += 1;

        for (int i = 1; i < snake.snake_length; i++) {
            if (eq_point(snake.segments[0], snake.segments[i])) game_over = true;
        }
        if (snake.segments[0].x < 0 || snake.segments[0].x >= GRID_WIDTH || snake.segments[0].y < 0 || snake.segments[0].y >= GRID_HEIGHT) game_over = true;

        if (game_over) {
            centered_message("GAME OVER", COLOR_RED, 0);
            return;
        }

        if (eq_point(snake.segments[0], apple)) {
            snake.segments[snake.snake_length] = snake.segments[snake.snake_length - 1];
            snake.snake_length++;

            while (!random_apple());
        }

        last_direction = direction;
        render_grid();
    }
}

taskmgr_program_t program_snake = {
    .init = program_snake_init,
    .run = program_snake_run,
    .name = "snake",
    .description = "snake game (play with W A S D, press ESC to quit)",
    .clears_screen = true,
};
