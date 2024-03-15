//
// Created by yang on 24-3-15.
//

#ifndef MYOS_ARENA_H
#define MYOS_ARENA_H

#include "../include/types.h"
#include "../include/list.h"

#define DESC_COUNT 7

typedef list_node_t block_t; // 内存块

// 内存描述符
typedef struct arena_descriptor_t
{
    u32 total_block;  // 一页内存分成了多少块
    u32 block_size;   // 块大小
    list_t free_list; // 空闲列表
} arena_descriptor_t;

// 一页或多页内存
typedef struct arena_t
{
    arena_descriptor_t *desc; // 该 arena 的描述符
    u32 count;                // 当前剩余多少块 或 页数
    u32 large;                // 表示是不是超过 1024 字节
    u32 magic;                // 魔数
} arena_t;

void *kmalloc(size_t size);
void kfree(void *ptr);

#endif //MYOS_ARENA_H
