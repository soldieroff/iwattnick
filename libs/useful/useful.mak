# This library provides various unclassified utility functions

LIBS += useful
DESCRIPTION.useful = A library with useful mostly hardware-independent functions
TARGETS.useful = useful$L

SRC.stm32.useful$L := $(wildcard libs/useful/thumb/*.S)
SRC.useful$L := $(wildcard libs/useful/*.c) $(SRC.$(MCU).useful$L)
CFLAGS.useful$L = -Iinclude/useful
