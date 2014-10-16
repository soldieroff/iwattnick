TESTS += tvld
DESCRIPTION.tvld = Default STM32VL-DISCOVERY board firmware
FLASH.TARGETS += tvld

TARGETS.tvld = tvld$E
SRC.tvld$E = $(wildcard tests/stm32vl_discovery/tvld/*.c)
LIBS.tvld$E = hardware$L spld$L CMSIS$L
