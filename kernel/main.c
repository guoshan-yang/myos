#include "../include/io.h"
#include "../include/types.h"
#include "../include/string.h"
#include "../include/console.h"
#include "../include/kernel.h"
#include "../include/debug.h"
#include "../include/gdt.h"
#include "../include/task.h"
#include "../include/idt.h"

extern void clock_init();

void kernel_init(){

    console_init();
    gdt_init();
    interrupt_init();
//    task_init();

    clock_init();

    asm volatile(
            "sti");

    while (true){

    }
}