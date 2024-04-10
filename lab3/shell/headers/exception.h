#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#define IRQ_BASIC_PENDING		((volatile unsigned int*)(PHY_BASE+0x0000B200))
#define IRQ_PENDING_1	    	((volatile unsigned int*)(PHY_BASE+0x0000B204))
#define IRQ_PENDING_2	    	((volatile unsigned int*)(PHY_BASE+0x0000B208))
#define FIQ_CONTROL	        	((volatile unsigned int*)(PHY_BASE+0x0000B20C))
#define ENABLE_IRQS_1	    	((volatile unsigned int*)(PHY_BASE+0x0000B210))
#define ENABLE_IRQS_2		    ((volatile unsigned int*)(PHY_BASE+0x0000B214))
#define ENABLE_BASIC_IRQS		((volatile unsigned int*)(PHY_BASE+0x0000B218))
#define DISABLE_IRQS_1		    ((volatile unsigned int*)(PHY_BASE+0x0000B21C))
#define DISABLE_IRQS_2		    ((volatile unsigned int*)(PHY_BASE+0x0000B220))
#define DISABLE_BASIC_IRQS		((volatile unsigned int*)(PHY_BASE+0x0000B224))
#define CORE0_TIMER_IRQ_CTRL	((volatile unsigned int*)(0x40000040))
#define CORE0_INTERRUPT_SOURCE	((volatile unsigned int*)(0x40000060))

void enable_interrupt(void);
void disable_interrupt(void);

void invalid_exception_handler(void);
void sync_handler(void);
void irq_handler(void);

void sync_el1h_handler(void);
void irq_el1h_handler(void);
void sync_64_handler(void);
void irq_64_handler(void);

#endif