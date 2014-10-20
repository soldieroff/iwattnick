TESTS += trcc
DESCRIPTION.trcc = System clock configuration demo
FLASH.TARGETS += trcc
IHEX.TARGETS += trcc

TARGETS.trcc = trcc$E
CFLAGS.trcc += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/trcc
SRC.trcc$E = $(wildcard tests/stm32vl_discovery/trcc/*.c)
LIBS.trcc$E = hardware$L spld$L CMSIS$L
