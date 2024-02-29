//
// Created by yang on 24-2-28.
//

#include "../include/gdt.h"
#include "../include/string.h"
#include "../include/debug.h"

descriptor_t gdt[GDT_SIZE]; // 内核全局描述符表
pointer_t gdt_ptr;          // 内核全局描述符表指针

// 初始化内核全局描述符表, 入内核之后重新设置gdt,因为在loader里面的设置的那块内存有可能会被覆盖
void gdt_init()
{
    DEBUGK("init gdt!!!\n");

    asm volatile("sgdt gdt_ptr");

    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limit + 1);

    gdt_ptr.base = (u32)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;
    asm volatile("lgdt gdt_ptr\n");
}
