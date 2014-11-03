LIBS += mudbus
DESCRIPTION.mudbus = A simple protocol for serial communications
TARGETS.mudbus = mudbus$L
SRC.mudbus$L := $(wildcard libs/mudbus/*.c) $(wildcard libs/mudbus/$(MCU)/*.c)
