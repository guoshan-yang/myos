//
// Created by yang on 24-3-6.
//


#include "../include/idt.h"
#include "../include/assert.h"
#include "../include/debug.h"
#include "../include/syscall.h"
#include "../include/task.h"
#include "../include/console.h"
#include "../include/memory.h"
#include "../include/device.h"
#include "../include/string.h"

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define SYSCALL_SIZE 256

handler_t syscall_table[SYSCALL_SIZE];

void syscall_check(u32 nr)
{
    if (nr >= SYSCALL_SIZE)
    {
        panic("syscall nr error!!!");
    }
}

static void sys_default()
{
    panic("syscall not implemented!!!");
}

task_t *task = NULL;


static u32 sys_test()
{
    char ch;
    device_t *device;

//    device = device_find(DEV_KEYBOARD, 0);
//    assert(device);
//    device_read(device->dev, &ch, 1, 0, 0);
//
//    device = device_find(DEV_CONSOLE, 0);
//    assert(device);
//    device_write(device->dev, &ch, 1, 0, 0);

    void *buf = (void *)alloc_kpage(1);

    device = device_find(DEV_IDE_PART, 0);
    assert(device);

    memset(buf, running_task()->pid, 512);

    device_request(device->dev, buf, 1, running_task()->pid, 0, REQ_WRITE);

    free_kpage((u32)buf, 1);

    return 255;
}

extern int32 console_write();

extern void task_yield();

int32 sys_write(fd_t fd, char *buf, u32 len) {
    if (fd == stdout || fd == stderr)
    {
        return console_write(NULL, buf, len);
    }
    // todo
    panic("write!!!!");
    return 0;
}

extern time_t sys_time();

void syscall_init()
{
    for (size_t i = 0; i < SYSCALL_SIZE; i++)
    {
        syscall_table[i] = sys_default;
    }

    syscall_table[SYS_NR_TEST] = sys_test;

    syscall_table[SYS_NR_EXIT] = task_exit;
    syscall_table[SYS_NR_FORK] = task_fork;
    syscall_table[SYS_NR_WAITPID] = task_waitpid;

    syscall_table[SYS_NR_SLEEP] = task_sleep;
    syscall_table[SYS_NR_YIELD] = task_yield;

    syscall_table[SYS_NR_GETPID] = sys_getpid;
    syscall_table[SYS_NR_GETPPID] = sys_getppid;

    syscall_table[SYS_NR_BRK] = sys_brk;
    syscall_table[SYS_NR_WRITE] = sys_write;

    syscall_table[SYS_NR_TIME] = sys_time;
}

