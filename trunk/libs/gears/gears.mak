# This library provides various unclassified utility functions

LIBS += gears
DESCRIPTION.gears = A math library for working with fixed-point numbers
TARGETS.gears = gears$L
SRC.gears$L := $(wildcard libs/gears/*.c)

# makedep doesn't catch this dependency
$(OUT)libs/gears/dma.o: libs/gears/dmafun.h
