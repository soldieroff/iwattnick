TESTS += tsystick
DESCRIPTION.tsystick = Configuring the SysTick timebase demo
FLASH.TARGETS += tsystick
IHEX.TARGETS += tsystick

TARGETS.tsystick = tsystick$E
CFLAGS.tsystick += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/examples/tsystick
SRC.tsystick$E = $(wildcard tests/stm32vl_discovery/examples/tsystick/*.c)
LIBS.tsystick$E = hardware$L spld$L CMSIS$L
