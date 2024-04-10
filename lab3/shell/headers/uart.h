#ifndef __UART_H_
#define __UART_H_

#define PHY_BASE        0x3f000000
#define BUS_BASE        0x7e000000

#define AUX_IRQ         (volatile unsigned int*)(PHY_BASE+0x00215000)
#define AUX_ENABLES     (volatile unsigned int*)(PHY_BASE+0x00215004)
#define AUX_MU_IO_REG   (volatile unsigned int*)(PHY_BASE+0x00215040)
#define AUX_MU_IER_REG  (volatile unsigned int*)(PHY_BASE+0x00215044)
#define AUX_MU_IIR_REG  (volatile unsigned int*)(PHY_BASE+0x00215048)
#define AUX_MU_LCR_REG  (volatile unsigned int*)(PHY_BASE+0x0021504c)
#define AUX_MU_MCR_REG  (volatile unsigned int*)(PHY_BASE+0x00215050)
#define AUX_MU_LSR_REG  (volatile unsigned int*)(PHY_BASE+0x00215054)
#define AUX_MU_MSR_REG  (volatile unsigned int*)(PHY_BASE+0x00215058)
#define AUX_MU_SCRATCH  (volatile unsigned int*)(PHY_BASE+0x0021505c)
#define AUX_MU_CNTL_REG (volatile unsigned int*)(PHY_BASE+0x00215060)
#define AUX_MU_STAT_REG (volatile unsigned int*)(PHY_BASE+0x00215064)
#define AUX_MU_BAUD_REG (volatile unsigned int*)(PHY_BASE+0x00215068)
#define ENABLE_IRQS_1   (volatile unsigned int*)(PHY_BASE+0x0000b210)

#define GPFSEL1         (volatile unsigned int*)(PHY_BASE+0x00200004)
#define GPPUD           (volatile unsigned int*)(PHY_BASE+0x00200094)
#define GPPUDCLK0       (volatile unsigned int*)(PHY_BASE+0x00200098)
#define GPPUDCLK1       (volatile unsigned int*)(PHY_BASE+0x0020009c)

#define RX_INTERRUPT                0x01
#define TX_INTERRUPT                0x02
#define AUXINIT_BIT_POSITION        1<<29

#define UART_BUFFER_SIZE            1024
#define UART_RX_BUFFER_SIZE         UART_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE         UART_BUFFER_SIZE

void init(void);
char receive(void);
void send(char);
void display(char*);
void display_hex(unsigned int);

void init_uart_async_ind(void);
void enable_uart_interrupt(void);
void disable_uart_interrupt(void);
void enable_uart_tx_interrupt(void);
void disable_uart_tx_interrupt(void);
void enable_uart_rx_interrupt(void);
void disable_uart_rx_interrupt(void);

void irq_uart_handler(void);

#endif
