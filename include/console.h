//
// Created by yang on 24-2-27.
//

#ifndef MYOS_CONSOLE_H
#define MYOS_CONSOLE_H

#include "types.h"

void console_init();
void console_clear();
void console_write(char *buf, u32 count);

#endif //MYOS_CONSOLE_H
