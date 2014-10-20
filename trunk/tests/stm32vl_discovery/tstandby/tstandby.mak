TESTS += tstandby
DESCRIPTION.tstandby = STANDBY mode usage example
FLASH.TARGETS += tstandby

TARGETS.tstandby = tstandby$E
CFLAGS.tstandby += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tstandby
SRC.tstandby$E = $(wildcard tests/stm32vl_discovery/tstandby/*.c)
LIBS.tstandby$E = hardware$L spld$L CMSIS$L
