//
// Created by yang on 24-2-29.
//

#ifndef MYOS_RTC_H
#define MYOS_RTC_H

#include "types.h"
void set_alarm(u32 secs);
u8 cmos_read(u8 addr);
void cmos_write(u8 addr, u8 value);

#endif //MYOS_RTC_H
