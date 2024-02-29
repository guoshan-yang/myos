//
// Created by yang on 24-2-28.
//

#include "../include/debug.h"
#include "../include/stdarg.h"
#include "../include/stdio.h"
#include "../include/kernel.h"

static char buf[1024];

void debugk(char *file, int line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    printk("[%s] [%d] %s", file, line, buf);
}