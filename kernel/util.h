#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define UTIL_32_LOW_16(val) ((uint16_t)(val & 0xFFFF))
#define UTIL_32_HIGH_16(val) ((uint16_t)((val >> 16) & 0xFFFF))

uint8_t util_in(uint16_t port);
void util_out(uint16_t port, uint8_t data);
size_t util_str_len(char *str);
bool util_str_cmp(char *str1, char *str2);

#endif