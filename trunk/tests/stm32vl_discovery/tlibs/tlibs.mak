TESTS += tlibs
DESCRIPTION.tlibs = Test for some library functions
FLASH.TARGETS += tlibs
IHEX.TARGETS += tlibs

TARGETS.tlibs = tlibs$E
SRC.tlibs$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tlibs/*.c)
LIBS.tlibs$E = CMSIS$L gears$L useful$L
