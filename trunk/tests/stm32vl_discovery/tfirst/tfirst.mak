TESTS += tfirst
DESCRIPTION.tfirst = My First Program for STM32 :)
FLASH.TARGETS += tfirst
IHEX.TARGETS += tfirst

TARGETS.tfirst = tfirst$E
SRC.tfirst$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tfirst/*.c)
LIBS.tfirst$E = CMSIS$L gears$L useful$L
