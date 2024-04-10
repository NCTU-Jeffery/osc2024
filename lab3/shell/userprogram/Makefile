SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Iinclude -fno-stack-protector -Wall -Wextra -Wpedantic -g  -O2 -ffreestanding -nostdinc -nostdlib -Wno-language-extension-token
CFLAGS += -I../../../opt/lib/gcc/aarch64-linux-gnu/12.2.0/include
all: up1.img

cpio: 
	find . | cpio -o -H newc > ../initramfs.cpio

up1.o: up1.S
	aarch64-linux-gnu-gcc $(CFLAGS) -c up1.S -o up1.o

up1.img: up1.o
	aarch64-linux-gnu-ld up1.o -T linker.ld -o up1.elf
	aarch64-linux-gnu-objcopy -O binary up1.elf up1.img