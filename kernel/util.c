#include "util.h"

uint8_t util_in(uint16_t port) {
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void util_out(uint16_t port, uint8_t data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

size_t util_str_len(char *str) {
    size_t len = 0;
    while (*str++) len++;
    return len;
}

bool util_str_cmp(char *str1, char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) return false;
        str1++;
        str2++;
    }
    return *str1 == *str2;
}
