#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 25

void display_init();
void display_char(char c);
void display_hex(uint8_t *buffer, uint32_t size);
void display_ln(char *str);
void display_str(char *str);
void display_clear();
void display_set_line(char *str, size_t row);

#endif