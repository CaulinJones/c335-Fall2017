TEMPLATEROOT = ../fall2017-lab

#complilation flags for gdb
CFLAGS  = -O1 -g
ASFLAGS = -g

#obj files

OBJS= $(STARTUP) newwc.o\
		f3d_led.o\
		f3d_user_btn.o\
		f3d_uart.o\
		stm32f30x_rcc.o\
		stm32f30x_gpio.o\
		stm32f30x_usart.o\
		newlib_stubs.o

vpath %.c .
include $(TEMPLATEROOT)/c335.mk
