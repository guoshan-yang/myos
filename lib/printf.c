//
// Created by yang on 24-3-15.
//
#include "../include/stdarg.h"
#include "../include/stdio.h"
#include "../include/syscall.h"

static char buf[1024];

int printf(const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);

    i = vsprintf(buf, fmt, args);

    va_end(args);

    write(STDOUT_FILENO, buf, i);

    return i;
}
