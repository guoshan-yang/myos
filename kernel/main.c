extern void interrupt_init();
extern void clock_init();
extern void time_init();
extern void rtc_init();
extern void clock_init();
extern void memory_map_init();
extern void mapping_init();

void kernel_init(){

    interrupt_init();
//    task_init();

//    clock_init();

    memory_map_init();
    mapping_init();
//    time_init();
//    rtc_init();

    bitmap_tests();

    asm volatile(
            "sti");

    while (1);
}