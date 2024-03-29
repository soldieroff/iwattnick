TESTS += tiwdg
DESCRIPTION.tiwdg = IWDG watchdog timer demo
FLASH.TARGETS += tiwdg
IHEX.TARGETS += tiwdg

TARGETS.tiwdg = tiwdg$E
CFLAGS.tiwdg += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/examples/tiwdg
SRC.tiwdg$E = $(wildcard tests/stm32vl_discovery/examples/tiwdg/*.c)
LIBS.tiwdg$E = hardware$L spld$L CMSIS$L
