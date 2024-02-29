//
// Created by yang on 24-2-28.
//

#ifndef MYOS_STDARG_H
#define MYOS_STDARG_H

typedef char *va_list;

#define va_start(ap, v) (ap = (va_list)&v + sizeof(char *))
#define va_arg(ap, t) (*(t *)((ap += sizeof(char *)) - sizeof(char *)))
#define va_end(ap) (ap = (va_list)0)

#endif //MYOS_STDARG_H
