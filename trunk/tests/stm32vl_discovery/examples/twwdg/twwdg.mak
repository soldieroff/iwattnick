TESTS += twwdg
DESCRIPTION.twwdg = Default STM32VL-DISCOVERY board firmware
FLASH.TARGETS += twwdg
IHEX.TARGETS += twwdg

TARGETS.twwdg = twwdg$E
CFLAGS.twwdg += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/examples/twwdg
SRC.twwdg$E = $(wildcard tests/stm32vl_discovery/examples/twwdg/*.c)
LIBS.twwdg$E = hardware$L spld$L CMSIS$L
