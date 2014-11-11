LIBS += spld
DESCRIPTION.spld = Standard Peripherals Library Drivers

TARGETS.spld = spld$L
CFLAGS.spld += -include libs/spld/spld_conf.h
SRC.spld$L = $(wildcard libs/spld/*.c)
CFLAGS.spld$L += -Iinclude/spld
