//
// Created by yang on 24-2-29.
//
#include "types.h"

#ifndef MYOS_STDLIB_H
#define MYOS_STDLIB_H

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

u8 bcd_to_bin(u8 value);
u8 bin_to_bcd(u8 value);

u32 div_round_up(u32 num, u32 size);

int atoi(const char *str);

#endif //MYOS_STDLIB_H
