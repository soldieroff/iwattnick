TESTS += tstandby
DESCRIPTION.tstandby = STANDBY mode usage example
FLASH.TARGETS += tstandby
IHEX.TARGETS += tstandby

TARGETS.tstandby = tstandby$E
CFLAGS.tstandby += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/examples/tstandby
SRC.tstandby$E = $(wildcard tests/stm32vl_discovery/examples/tstandby/*.c)
LIBS.tstandby$E = hardware$L spld$L CMSIS$L
