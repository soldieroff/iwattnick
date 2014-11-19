# This library provides various unclassified utility functions

LIBS += gears
DESCRIPTION.gears = A thin shim around low-level platform hardware
TARGETS.gears = gears$L

SRC.gears$L := $(wildcard libs/gears/*.c) $(wildcard libs/gears/$(MCU)/*.c)
LIBS.gears$L = CMSIS$L useful$L
CFLAGS.gears$L = -Iinclude/gears -Iinclude/gears/$(MCU)
