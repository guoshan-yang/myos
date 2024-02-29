#include "../include/io.h"
#include "../include/types.h"
#include "../include/string.h"
#include "../include/console.h"
#include "../include/kernel.h"
#include "../include/debug.h"
#include "../include/gdt.h"
#include "../include/task.h"
#include "../include/idt.h"

char message[] = "hello os !!! \n";
char buf[1024];

void kernel_init(){

    console_init();
    gdt_init();
    interrupt_init();
    task_init();

    asm volatile(
            "sti");
    return;
}