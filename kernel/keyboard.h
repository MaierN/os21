#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdint.h"
#include "stdbool.h"

void keyboard_init();
bool keyboard_read(char *letter);

#endif