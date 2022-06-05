#include "rand.h"

static uint32_t state = 0;

void srand(uint32_t seed) {
    state = seed;
}

uint32_t rand() {
    state = state * 1103515245 + 12345;
    return (uint32_t)(state / 65536) % 32768;
}
