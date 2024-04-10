#ifndef __TIMER_H
#define __TIMER_H

#define CORE0_TIMER_IRQ_CTRL 0x40000040

void irq_timer_handler(void);
void disable_timeout(void);

#endif