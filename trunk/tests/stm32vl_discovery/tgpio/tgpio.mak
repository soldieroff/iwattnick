TESTS += tgpio
DESCRIPTION.tgpio = GPIO usage demo
FLASH.TARGETS += tgpio
IHEX.TARGETS += tgpio

TARGETS.tgpio = tgpio$E
CFLAGS.tgpio += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tgpio
SRC.tgpio$E = $(wildcard tests/stm32vl_discovery/tgpio/*.c)
LIBS.tgpio$E = hardware$L spld$L CMSIS$L
