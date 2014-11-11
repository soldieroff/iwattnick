TESTS += tgears
DESCRIPTION.tgears = Test for some libgears functions
FLASH.TARGETS += tgears
IHEX.TARGETS += tgears

TARGETS.tgears = tgears$E
SRC.tgears$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tgears/*.c)
LIBS.tgears$E = CMSIS$L gears$L
