//
// Created by yang on 24-3-7.
//

#include "../include/idt.h"
#include "../include/syscall.h"
#include "../include/debug.h"
#include "../include/task.h"
#include "../include/stdio.h"
#include "../include/arena.h"

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

void idle_thread()
{
    set_interrupt_state(true);
    u32 counter = 0;
    while (true)
    {
//        LOGK("idle task.... %d\n", counter++);
        asm volatile(
                "sti\n" // 开中断
                "hlt\n" // 关闭 CPU，进入暂停状态，等待外中断的到来
                );
        yield(); // 放弃执行权，调度执行其他任务
    }
}

static void user_init_thread()
{
    int status;
    while (true)
    {
//        pid_t pid = fork();
//
//        if (pid)
//        {
//            printf("fork after parent %d, %d, %d\n", pid, getpid(), getppid());
//            // sleep(1000);
//            pid_t child = waitpid(pid, &status);
//            printf("wait pid %d status %d %d\n", child, status, time());
//        }
//        else
//        {
//            printf("fork after child %d, %d, %d\n", pid, getpid(), getppid());
//            sleep(1000);
//            exit(0);
//        }

        sleep(1000);
    }
}

extern task_t *running_task();

#include "../include/mutex.h"

lock_t lock;

extern u32 keyboard_read(char *buf, u32 count);

void init_thread()
{
    char temp[100]; // 为栈顶有足够的空间

    set_interrupt_state(true);
    test();

    task_to_user_mode(user_init_thread);
}

void test_thread()
{
    set_interrupt_state(true);
    u32 counter = 0;

    while (true)
    {
//        printf("test thread %d %d %d...\n", getpid(), getppid(), counter++);
        // LOGK("test task %d....\n", counter++);
        // BMB;
        sleep(2000);
    }
}