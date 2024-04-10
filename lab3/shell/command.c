#include "headers/command.h"
#include "headers/uart.h"
#include "headers/mailbox.h"
#include "headers/reboot.h"
#include "headers/cpio.h"
#include "headers/allocator.h"
#include "headers/utils.h"
#include "headers/timer.h"

void help()
{
    display("help:      Print this help munu.\n");
    display("hello:     Print HELLO WORLD!\n");
    display("info:      Print Hardware information.\n");
    display("reboot:    Reboot the device.\n");
    display("ls:        List all files in this directory.\n");
    display("cat:       Print the content of file.\n");
    display("malloc:    Return the address allocated.\n");
    display("lp:        Load the program.\n");
    display("dt:        Disable the timeout.\n");
    display("async:     Demo the async function.\n");
}

void hello()
{
    display("HELLO WORLD!\n");
}

void info()
{
    get_board_revision();
    get_arm_memory();
}

void reboot()
{
    display("Rebooting...\n");
    reset(100);
}

void ls()
{
    cpio_ls();
}

void cat(char* filename)
{
    cpio_cat(filename);
}

void malloc(char *size_str)
{
    unsigned int size = atoi(size_str, strlen(size_str));
    unsigned int *ptr = (unsigned int *)(simple_malloc(size));
    char addr[8];
    bin2hex((unsigned int)ptr, addr);
    display("Address: 0x"); display(addr); display(" with "); display(size_str); display(" bytes.\n");
}

void load_program(char *filename)
{
    cpio_load_program(filename);
}

// disable the timeout
void dt()
{
    disable_timeout();
}

void async()
{
    display("This is the demo of uart async rx and tx.\n");
    enable_uart_interrupt();



    disable_uart_interrupt();
}

void undefined()
{
    display("Command not found.\n");
}