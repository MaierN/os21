#include "display.h"
#include "util.h"

#define DISPLAY_PTR 0xb8000

#define GET_DISPLAY_BUFFER(c, r) (*(uint8_t*)(DISPLAY_PTR + 2 * (c) + 2 * DISPLAY_WIDTH * (r)))
#define SET_DISPLAY_BUFFER(c, r, val) *(uint8_t*)(DISPLAY_PTR + 2 * (c) + 2 * DISPLAY_WIDTH * (r)) = val

void display_init() {
    util_out(0x3D4, 0x0A);
    util_out(0x3D5, 0x20);
}

static char nibble_num_to_char(char nibble) {
    if (nibble < 10) nibble += '0';
    else nibble += 'A' - 10;
    return nibble;
}

static uint32_t cursor_pos_x = 0;

void display_char(char c) {
    bool scroll = c == '\n' || cursor_pos_x == DISPLAY_WIDTH;
    if (scroll) {
        cursor_pos_x = 0;
        for (size_t row = 0; row < DISPLAY_HEIGHT; row++) {
            for (size_t col = 0; col < DISPLAY_WIDTH; col++) {
                if (row < DISPLAY_HEIGHT - 1) SET_DISPLAY_BUFFER(col, row, GET_DISPLAY_BUFFER(col, row + 1));
                else SET_DISPLAY_BUFFER(col, row, 0x00);
            }
        }
    }
    if (c != '\n') {
        SET_DISPLAY_BUFFER(cursor_pos_x++, DISPLAY_HEIGHT - 1, c);
    }
}

void display_hex(uint8_t *buffer, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        uint8_t b = buffer[i];
        display_char('0');
        display_char('x');
        display_char(nibble_num_to_char(b >> 4));
        display_char(nibble_num_to_char(b & 0xf));
        display_char(' ');
    }
}

void display_str(char *str) {
    while (*str != 0) {
        display_char(*(str++));
    }
}

void display_ln(char *str) {
    while (*str != 0) {
        display_char(*(str++));
    }
    display_char('\n');
}

void display_clear() {
    for (size_t col = 0; col < DISPLAY_WIDTH; col++) {
        for (size_t row = 0; row < DISPLAY_HEIGHT; row++) {
            SET_DISPLAY_BUFFER(col, row, 0x00);
        }
    }
}

void display_set_line(char *str, size_t row) {
    size_t i = 0;
    for (; i < DISPLAY_WIDTH && str[i] != 0; i++) {
        SET_DISPLAY_BUFFER(i, row, str[i]);
    }
    for (; i < DISPLAY_WIDTH; i++) {
        SET_DISPLAY_BUFFER(i, row, 0x00);
    }
}

void display_set_char(size_t col, size_t row, char c) {
    SET_DISPLAY_BUFFER(col, row, c);
}
