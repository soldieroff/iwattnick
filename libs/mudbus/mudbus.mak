LIBS += mudbus
DESCRIPTION.mudbus = A simple flexible protocol for serial communications
TARGETS.mudbus = mudbus$L
SRC.mudbus$L := $(wildcard libs/mudbus/*.c) $(wildcard libs/mudbus/$(MCU)/*.c)
LIBS.mudbus$L = useful$L gears$L
CFLAGS.mudbus$L = -Iinclude/mudbus -Iinclude/mudbus/$(MCU)
