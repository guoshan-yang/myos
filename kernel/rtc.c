//
// Created by yang on 24-2-29.
//
// 实时时钟
#include "../include/types.h"
#include "../include/debug.h"
#include "../include/idt.h"
#include "../include/io.h"
#include "../include/time.h"
#include "../include/assert.h"
#include "../include/stdlib.h"
#include "../include/rtc.h"

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define CMOS_ADDR 0x70 // CMOS 地址寄存器
#define CMOS_DATA 0x71 // CMOS 数据寄存器

#define CMOS_SECOND 0x01
#define CMOS_MINUTE 0x03
#define CMOS_HOUR 0x05

#define CMOS_A 0x0a
#define CMOS_B 0x0b
#define CMOS_C 0x0c
#define CMOS_D 0x0d
#define CMOS_NMI 0x80

// 读 cmos 寄存器的值
u8 cmos_read(u8 addr)
{
    out_byte(CMOS_ADDR, CMOS_NMI | addr);
    return in_byte(CMOS_DATA);
};

// 写 cmos 寄存器的值
void cmos_write(u8 addr, u8 value)
{
    out_byte(CMOS_ADDR, CMOS_NMI | addr);
    out_byte(CMOS_DATA, value);
}

static u32 volatile counter = 0;

// 实时时钟中断处理函数
void rtc_handler(int vector)
{
    // 实时时钟中断向量号
    assert(vector == 0x28);

    // 向中断控制器发送中断处理完成的信号
    send_eoi(vector);

    // 读 CMOS 寄存器 C，允许 CMOS 继续产生中断
    cmos_read(CMOS_C);

    set_alarm(3);

    LOGK("rtc handler %d...\n", counter++);
}

// 设置 secs 秒后发生实时时钟中断
void set_alarm(u32 secs)
{
    tm time;
    time_read(&time);

    u8 sec = secs % 60;
    secs /= 60;
    u8 min = secs % 60;
    secs /= 60;
    u32 hour = secs;

    time.tm_sec += sec;
    if (time.tm_sec >= 60)
    {
        time.tm_sec %= 60;
        time.tm_min += 1;
    }

    time.tm_min += min;
    if (time.tm_min >= 60)
    {
        time.tm_min %= 60;
        time.tm_hour += 1;
    }

    time.tm_hour += hour;
    if (time.tm_hour >= 24)
    {
        time.tm_hour %= 24;
    }

    cmos_write(CMOS_HOUR, bin_to_bcd(time.tm_hour));
    cmos_write(CMOS_MINUTE, bin_to_bcd(time.tm_min));
    cmos_write(CMOS_SECOND, bin_to_bcd(time.tm_sec));
}

void rtc_init()
{
    u8 prev;

    // cmos_write(CMOS_B, 0b01000010); // 打开周期中断
    cmos_write(CMOS_B, 0b00100010); // 打开闹钟中断
    cmos_read(CMOS_C); // 读 C 寄存器，以允许 CMOS 中断

    set_alarm(10);

    // 设置中断频率
    out_byte(CMOS_A, (in_byte(CMOS_A) & 0xf) | 0b1110);

    set_interrupt_handler(IRQ_RTC, rtc_handler);
    set_interrupt_mask(IRQ_RTC, true);
    set_interrupt_mask(IRQ_CASCADE, true);
}
