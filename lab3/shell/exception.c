#include "headers/exception.h"
#include "headers/uart.h"
#include "headers/utils.h"
#include "headers/timer.h"

#define CNTPSIRQ_BIT_POSITION 0x02
#define AUXINIT_BIT_POSITION 1<<29

void enable_interrupt()
{
    asm volatile("msr DAIFClr, 0xf"); // Set DAIF to 0
}

void disable_interrupt()
{
    asm volatile("msr DAIFSet, 0xf"); // Set DAIF to 1
}

void invalid_exception_handler()
{
    // display("Invalid exception raised.\n");
}

void sync_el1h_handler()
{
    // display("SYNC_EL1H_64\n");
    sync_handler();
}

void irq_el1h_handler()
{
    // display("IRQ_EL1H\n");
    irq_handler();
}

void sync_64_handler()
{
    // display("SYNC_64\n");
    // sync_handler();
}

void irq_64_handler()
{
    // display("IRQ_64\n");
    irq_handler();
}

void sync_handler()
{
    display("Synchronous Exception Handler.\n\n");

    unsigned long long spsr, elr, esr;
    asm("mrs %0, spsr_el1\n":"=r"(spsr));
    asm("mrs %0, elr_el1\n":"=r"(elr));
    asm("mrs %0, esr_el1":"=r"(esr));
    
    char temp[8];
    bin2hex(spsr, temp);
    display("spsr_el1: 0x"); display(temp);
    
    bin2hex(elr, temp);
    display("\nelr_el1: 0x"); display(temp);
    
    bin2hex(esr, temp);
    display("\nesr_el1: 0x"); display(temp);
    display("\n\n");
}

void irq_handler()
{
    // Disable interrupts
    asm("msr DAIFSet, 0xf");

    if((*CORE0_INTERRUPT_SOURCE) & CNTPSIRQ_BIT_POSITION)
    {
        irq_timer_handler();
    }
    else if((*IRQ_PENDING_1) & AUXINIT_BIT_POSITION)
    {
        irq_uart_handler();
    }

    asm volatile("msr DAIFClr, 0xf");
}