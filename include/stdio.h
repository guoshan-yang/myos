//
// Created by yang on 24-2-28.
//

#include "stdarg.h"

#ifndef MYOS_STDIO_H
#define MYOS_STDIO_H

typedef char *va_list;

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

#endif //MYOS_STDIO_H
