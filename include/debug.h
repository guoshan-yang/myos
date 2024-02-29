//
// Created by yang on 24-2-28.
//

#ifndef MYOS_DEBUG_H
#define MYOS_DEBUG_H

void debugk(char *file, int line, const char *fmt, ...);

#define BMB asm volatile("xchgw %bx, %bx") // bochs magic breakpoint
#define DEBUGK(fmt, args...) debugk(__BASE_FILE__, __LINE__, fmt, ##args)

#endif //MYOS_DEBUG_H
