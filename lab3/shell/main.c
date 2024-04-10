#include "headers/shell.h"
#include "headers/uart.h"
#include "headers/dtb.h"
#include "headers/timer.h"
#include "headers/utils.h"
#include "headers/exception.h"

extern void *_dtb_ptr;

void main()
{
    init();

    unsigned long t = 0;
    asm volatile("mrs %0, CurrentEL":"=r"(t));
    display("Current EL: "); display_hex(t>>2); display("\n");

    asm volatile("mov %0, sp":"=r"(t));
    display("Current stack pointer: "); display_hex(t); display("\n");

    fdt_traverse(initramfs_callback, _dtb_ptr);

    enable_interrupt();
    enable_uart_interrupt();

    // shell();
    // return;
}