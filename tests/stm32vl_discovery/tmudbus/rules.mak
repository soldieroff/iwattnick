TESTS += tmudbus
DESCRIPTION.tmudbus = Test the MudBus library
FLASH.TARGETS += tmudbus
IHEX.TARGETS += tmudbus

TARGETS.tmudbus = tmudbus$E
SRC.tmudbus$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tmudbus/*.c)
LIBS.tmudbus$E = CMSIS$L mudbus$L gears$L
