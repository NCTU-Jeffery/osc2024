#include "headers/timer.h"

void irq_timer_handler()
{
    display("In Timer Handler.\n");
    unsigned long long cntpct_el0 = 0;
    unsigned long long cntfrq_el0 = 0;
    
    asm volatile("mrs %0, cntpct_el0":"=r"(cntpct_el0));
    asm volatile("mrs %0, cntfrq_el0":"=r"(cntfrq_el0));

    unsigned long long sec = cntpct_el0 / cntfrq_el0;
    char sec_string[8]; bin2hex(sec, sec_string);
    display("Second after booting: 0x"); display(sec_string); display("\n");

    unsigned long long timeout = cntfrq_el0 * 2;
    asm volatile("msr cntp_tval_el0, %0"::"r"(timeout));
}

void disable_timeout()
{
    unsigned long long cntfrq_el0 = 0;
    asm volatile("mrs %0, cntfrq_el0":"=r"(cntfrq_el0));

    unsigned long long timeout = cntfrq_el0 << 12;
    asm volatile("msr cntp_tval_el0, %0"::"r"(timeout));
}
