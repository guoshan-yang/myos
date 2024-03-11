//
// Created by yang on 24-3-7.
//

#include "../include/idt.h"
#include "../include/syscall.h"
#include "../include/debug.h"
#include "../include/task.h"

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

extern task_t *running_task();

#include "../include/mutex.h"
spinlock_t lock;

void init_thread()
{
    spin_init(&lock);

    set_interrupt_state(true);
    u32 counter = 0;

    while (true)
    {
        spin_lock(&lock);
        task_t* task = running_task();
        LOGK("init task %d....%d...\n", counter++, task->jiffies);
        spin_unlock(&lock);
    }
}

void test_thread()
{
    set_interrupt_state(true);
    u32 counter = 0;


    while (true)
    {
        spin_lock(&lock);
        task_t* task = running_task();
        LOGK("test task %d....%d...\n", counter++, task->jiffies);
        sleep(100);
        spin_unlock(&lock);
    }
}