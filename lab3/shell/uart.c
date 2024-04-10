#include "headers/uart.h"
#include "headers/shell.h"
#include "headers/utils.h"

char uart_read_buffer[UART_RX_BUFFER_SIZE];
char uart_write_buffer[UART_TX_BUFFER_SIZE];
int uart_read_head = 0;
int uart_read_index = 0;
int uart_write_head = 0;
int uart_write_index = 0;

static unsigned int tx_r_ind;
static unsigned int tx_w_ind;
static unsigned int rx_r_ind;
static unsigned int rx_w_ind;

void init(void)
{
    // Since GPIO is set to alternate function, we need to disable the pull up/down.
    *GPPUD = 0; //let the GPIO pull down
    unsigned int r = 150; while(r--){ asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14) | (1<<15); //modify 14 and 15
    r = 150; while(r--){ asm volatile("nop"); }
    *GPPUDCLK0 = 0;
    r = 500; while(r--){asm volatile("nop"); }

    *AUX_ENABLES |= 1;          // enable Mini UART, which is on the bit 0.
    *AUX_MU_CNTL_REG = 0;       // bit 0/1 for rx/tx enable
    *AUX_MU_IER_REG = 0;        // interrupt enable register (currently don't need interrupt)
    *AUX_MU_LCR_REG = 3;        // data format and DLAB access 00/11 for 7/8-bit mode
    *AUX_MU_MCR_REG = 0;        // control the 'modem' signals
    *AUX_MU_BAUD_REG = 270;     // baud rate
    *AUX_MU_IIR_REG = 0xc6;     // interrupt status, bit 1/2 set clear the rx/tx FIFO
    *AUX_MU_CNTL_REG = 3;

    // p.92 describes how to set alternate functions for GPIO
    // for rpi: GPIO14:TX, GPIO15:RX, mini UART should set ALT5(010)
    unsigned int temp_reg = *GPFSEL1;   
    temp_reg &= ~(7<<12 | 7<<15);
    temp_reg |= (2<<12 | 2<<15);
    *GPFSEL1 = temp_reg;
}

char receive(void)
{
    char r;
    do{asm volatile("nop");}while(!(*AUX_MU_LSR_REG&0x01)); //bit 0 is for Data ready
    r = (char)(*AUX_MU_IO_REG);
    return r=='\r'?'\n':r;
}

void send(char r)
{
    unsigned int t = r;
    do{asm volatile("nop");}while(!(*AUX_MU_LSR_REG&0x20)); // bit 5/6 is for tx empty/idle
    *AUX_MU_IO_REG = t;
}

void display(char *string)
{
    while(!(*string == '\0'))
    {
        if(*string=='\n') send('\r');
        send(*string++);
    }
}

void display_hex(unsigned int x)
{
    char temp[8];
    bin2hex(x, temp);
    display("0x"); display(temp);
}

void init_uart_async_ind()
{
    rx_r_ind = 0;    rx_w_ind = 0;    tx_r_ind = 0;    tx_w_ind = 0;
}

void enable_uart_interrupt()
{
    enable_uart_rx_interrupt();
    enable_uart_tx_interrupt();
    *ENABLE_IRQS_1 |= AUXINIT_BIT_POSITION;
}

void disable_uart_interrupt()
{
    disable_uart_rx_interrupt();
    disable_uart_rx_interrupt();
    *ENABLE_IRQS_1 &= ~(AUXINIT_BIT_POSITION);
}

void enable_uart_tx_interrupt()
{
    *AUX_MU_IER_REG |= 2; // bit 1: Enable transmit interrupts.
}

void disable_uart_tx_interrupt()
{
    *AUX_MU_IER_REG &= ~(2);
}

void enable_uart_rx_interrupt()
{
    *AUX_MU_IER_REG |= 1; // bit 0: Enable receive interrupt.
}

void disable_uart_rx_interrupt()
{
    *AUX_MU_IER_REG &= ~(1);
}

void irq_uart_handler()
{
    // rx interrupt
    if((*AUX_MU_IIR_REG & 0x06) == 0x04) // IIR[2:1]=01: Receiver holds valid byte.
    {
        display("UART Handler - Receive\n");
        char data = (*AUX_MU_IO_REG) & 0xff;
        uart_read_buffer[uart_read_index++] = data;
        if(uart_read_index >= UART_RX_BUFFER_SIZE) uart_read_index = 0;

        (*AUX_MU_IER_REG) = (*AUX_MU_IER_REG | 0x2);
    }

    // tx interrupt
    if((*AUX_MU_IIR_REG & 0x06) == 0x02) // IIR[2:1]=10: Transmit holding register empty.
    {
        display("UART Handler - Transmit\n");
        if(uart_write_head != uart_write_index)
        {
            display("111\n");
            *AUX_MU_IO_REG = uart_write_buffer[uart_write_head++];
            if(uart_write_head >= UART_TX_BUFFER_SIZE) uart_write_head = 0;
        }
        else
        {
            display("222\n");
            *AUX_MU_IER_REG = (*AUX_MU_IER_REG) & ~(0x2);
            if(uart_read_buffer[uart_read_index-1] == '\r')
            {
                display("333\n");
                uart_read_buffer[uart_read_index-1] = '\0';
                uart_async_send(uart_read_buffer);
                parsing(uart_read_buffer);
                uart_read_index = 0;
                uart_write_index = 0;
                uart_write_head = 0;
            }
        }
    }
}