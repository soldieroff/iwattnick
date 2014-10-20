TESTS += tjtag
DESCRIPTION.tjtag = Demonstrates how to use JTAG pins as regular GPIO
FLASH.TARGETS += tjtag
IHEX.TARGETS += tjtag

TARGETS.tjtag = tjtag$E
CFLAGS.tjtag += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tjtag
SRC.tjtag$E = $(wildcard tests/stm32vl_discovery/tjtag/*.c)
LIBS.tjtag$E = hardware$L spld$L CMSIS$L
