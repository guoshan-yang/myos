//
// Created by yang on 24-3-6.
//

#ifndef MYOS_SYSCALL_H
#define MYOS_SYSCALL_H

#include "../include/types.h"

typedef enum syscall_t
{
    SYS_NR_TEST,
    SYS_NR_WRITE,
    SYS_NR_SLEEP,
    SYS_NR_YIELD,
} syscall_t;

u32 test();
void yield();
void sleep(u32 ms);
int32 write(fd_t fd, char *buf, u32 len);

#endif //MYOS_SYSCALL_H
