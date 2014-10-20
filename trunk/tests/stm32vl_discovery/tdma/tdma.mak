TESTS += tdma
DESCRIPTION.tdma = DMA transfer demo
FLASH.TARGETS += tdma

TARGETS.tdma = tdma$E
CFLAGS.tdma += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tdma
SRC.tdma$E = $(wildcard tests/stm32vl_discovery/tdma/*.c)
LIBS.tdma$E = hardware$L spld$L CMSIS$L