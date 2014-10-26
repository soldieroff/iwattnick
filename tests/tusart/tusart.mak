TESTS += tusart
DESCRIPTION.tusart = Test the USART library
FLASH.TARGETS += tusart
IHEX.TARGETS += tusart

TARGETS.tusart = tusart$E
SRC.tusart$E = $(wildcard tests/tusart/*.c)
LIBS.tusart$E = CMSIS$L gears$L
