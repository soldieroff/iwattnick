TESTS += tusart
DESCRIPTION.tusart = Test the USART library
FLASH.TARGETS += tusart
IHEX.TARGETS += tusart

TARGETS.tusart = tusart$E
SRC.tusart$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tusart/*.c)
LIBS.tusart$E = CMSIS$L gears$L
