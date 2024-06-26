#ifndef MYOS_TYPES_H
#define MYOS_TYPES_H

#define EOF -1 // END OF FILE

#define NULL ((void *)0) // 空指针

#define EOS '\0' // 字符串结尾

#define CONCAT(x, y) x##y
#define RESERVED_TOKEN(x, y) CONCAT(x, y)
#define RESERVED RESERVED_TOKEN(reserved, __LINE__)

#define bool _Bool
#define true 1
#define false 0

#define _packed __attribute__((packed)) // 用于定义特殊的结构体

// 用于省略函数的栈帧
#define _ofp __attribute__((optimize("omit-frame-pointer")))

#define _inline __attribute__((always_inline)) inline

typedef unsigned int size_t;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef int32 pid_t;
typedef int32 dev_t;

typedef u32 time_t;
typedef u32 idx_t;

typedef u16 mode_t; // 文件权限

typedef int32 fd_t;
typedef enum std_fd_t
{
    STDIN_FILENO,
    STDOUT_FILENO,
    STDERR_FILENO,
} std_fd_t;

typedef int32 off_t; // 文件偏移

// 内核魔数，用于校验错误
#define KERNEL_MAGIC 0x20220205

#endif //MYOS_TYPES_H