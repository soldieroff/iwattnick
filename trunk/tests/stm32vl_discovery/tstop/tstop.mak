TESTS += tstop
DESCRIPTION.tstop = STOP mode usage example
FLASH.TARGETS += tstop
IHEX.TARGETS += tstop

TARGETS.tstop = tstop$E
CFLAGS.tstop += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tstop
SRC.tstop$E = $(wildcard tests/stm32vl_discovery/tstop/*.c)
LIBS.tstop$E = hardware$L spld$L CMSIS$L
