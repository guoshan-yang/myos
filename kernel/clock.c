
/**
 * 时钟中断
 */
#include "../include/io.h"
#include "../include/idt.h"
#include "../include/debug.h"
#include "../include/assert.h"
#include "../include/task.h"

#define PIT_CHAN0_REG 0X40
#define PIT_CHAN2_REG 0X42
#define PIT_CTRL_REG 0X43

#define HZ 100
#define OSCILLATOR 1193182
#define CLOCK_COUNTER (OSCILLATOR / HZ)
#define JIFFY (1000 / HZ)

// 时间片计数器
u32 volatile jiffies = 0;
u32 jiffy = JIFFY;

extern void task_wakeup();

void clock_handler(int vector)
{
    assert(vector == 0x20);
    send_eoi(vector);

    task_wakeup(); // 唤醒睡眠结束的任务

    jiffies++;
//    DEBUGK("clock jiffies %d ...\n", jiffies);

    task_t *task = running_task();
    assert(task->magic == KERNEL_MAGIC);

    task->jiffies = jiffies;
    task->ticks--;
    if (!task->ticks)
    {
        schedule();
    }
}

extern u32 startup_time;

time_t sys_time()
{
    return startup_time + (jiffies * JIFFY) / 1000;
}

void pit_init()
{
    out_byte(PIT_CTRL_REG, 0b00110100);
    out_byte(PIT_CHAN0_REG, CLOCK_COUNTER & 0xff);
    out_byte(PIT_CHAN0_REG, (CLOCK_COUNTER >> 8) & 0xff);
}

void clock_init()
{
    pit_init();
    set_interrupt_handler(IRQ_CLOCK, clock_handler);
    set_interrupt_mask(IRQ_CLOCK, true);
}
