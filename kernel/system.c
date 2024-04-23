//
// Created by yang on 24-4-23.
//
#include "../include/task.h"

mode_t sys_umask(mode_t mask)
{
    task_t *task = running_task();
    mode_t old = task->umask;
    task->umask = mask & 0777;
    return old;
}
