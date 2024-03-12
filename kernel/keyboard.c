//
// Created by yang on 24-3-12.
//

#include "../include/idt.h"
#include "../include/io.h"
#include "../include/assert.h"
#include "../include/debug.h"

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CTRL_PORT 0x64

void keyboard_handler(int vector)
{
    assert(vector == 0x21);
    send_eoi(vector);                       // 发送中断处理完成信号
    u16 scancode = in_byte(KEYBOARD_DATA_PORT); // 从键盘读取按键信息扫描码
    LOGK("keyboard input 0x%x\n", scancode);
}

void keyboard_init()
{
    set_interrupt_handler(IRQ_KEYBOARD, keyboard_handler);
    set_interrupt_mask(IRQ_KEYBOARD, true);
}