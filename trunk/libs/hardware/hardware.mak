HARDWARE.FILES := $(wildcard libs/hardware/$(HARDWARE)/*.c)

ifneq ($(HARDWARE.FILES),)
LIBS += hardware
DESCRIPTION.hardware = $(HARDWARE) board specific library
DIR.INCLUDE.C += :include/hardware/$(HARDWARE)

TARGETS.hardware = hardware$L
SRC.hardware$L = $(HARDWARE.FILES)
LIBS.hardware$L = spld$L

include $(wildcard libs/hardware/$(HARDWARE)/*.mak)
endif
