//
// Created by yang on 24-2-28.
//

#ifndef MYOS_KERNEL_H
#define MYOS_KERNEL_H

// 内核魔数，用于校验错误
#define KERNEL_MAGIC 0x20220205

int printk(const char * fmt, ...);

#endif //MYOS_KERNEL_H
