TESTS += tmydma
DESCRIPTION.tmydma = Test my DMA access library
FLASH.TARGETS += tmydma
IHEX.TARGETS += tmydma

TARGETS.tmydma = tmydma$E
SRC.tmydma$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/tmydma/*.c)
LIBS.tmydma$E = CMSIS$L gears$L
