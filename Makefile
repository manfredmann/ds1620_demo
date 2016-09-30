BINARY = main

OBJS = hw.o ds1620.o

OPENCM3_DIR = ./lib/libopencm3

LDSCRIPT = ./lib/libopencm3/lib/stm32/f4/stm32f405x6.ld
LDLIBS += -lm

include Makefile_f4.include
