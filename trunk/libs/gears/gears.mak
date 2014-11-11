# This library provides various unclassified utility functions

LIBS += gears
DESCRIPTION.gears = A thin shim around low-level platform hardware
TARGETS.gears = gears$L

SRC.gears$L := $(wildcard libs/gears/*.c) $(wildcard libs/gears/$(MCU)/*.c)
LIBS.gears$L = CMSIS$L
CFLAGS.gears$L = -Iinclude/gears -Iinclude/gears/$(MCU)

# makedep doesn't catch this dependency
$(OUT)libs/gears/stm32/dma.o: libs/gears/stm32/dmafun.h
