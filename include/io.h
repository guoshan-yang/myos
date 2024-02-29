//
// Created by yang on 24-2-27.
//

#ifndef MYOS_IO_H
#define MYOS_IO_H

char in_byte(int port);
short in_word(int port);

void out_byte(int port, int v);
void out_word(int port, int v);

#endif //MYOS_IO_H
