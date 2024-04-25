//
// Created by yang on 24-4-25.
//

#include "../include/fs.h"
#include "../include/assert.h"
#include "../include/task.h"

#define FILE_NR 128

file_t file_table[FILE_NR];

// 从文件表中获取一个空文件
file_t *get_file()
{
    // 从3开始找，0,1,2表示标准输出，标准输入，标准错误
    for (size_t i = 3; i < FILE_NR; i++)
    {
        file_t *file = &file_table[i];
        if (!file->count)
        {
            file->count++;
            return file;
        }
    }
    panic("Exceed max open files!!!");
}

void put_file(file_t *file)
{
    assert(file->count > 0);
    file->count--;
    if (!file->count)
    {
        iput(file->inode);
    }
}

void file_init()
{
    for (size_t i = 0; i < FILE_NR; i++)
    {
        file_t *file = &file_table[i];
        file->mode = 0;
        file->count = 0;
        file->flags = 0;
        file->offset = 0;
        file->inode = NULL;
    }
}
