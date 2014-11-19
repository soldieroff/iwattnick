TESTS += tlcd
DESCRIPTION.tlcd = Test the ST7567-based LCD
FLASH.TARGETS += tlcd
IHEX.TARGETS += tlcd

TARGETS.tlcd = tlcd$E
SRC.tlcd$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tlcd/*.c)
LIBS.tlcd$E = CMSIS$L gears$L useful$L
