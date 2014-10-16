LIBS += spld
DESCRIPTION.spld = Standard Peripherals Library Drivers
DIR.INCLUDE.C += :include/spld

TARGETS.spld = spld$L
CFLAGS.spld += -include libs/spld/spld_conf.h
SRC.spld$L = $(wildcard libs/spld/*.c)
