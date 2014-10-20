TESTS += tsleep
DESCRIPTION.tsleep = SLEEP mode usage example
FLASH.TARGETS += tsleep

TARGETS.tsleep = tsleep$E
CFLAGS.tsleep += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tsleep
SRC.tsleep$E = $(wildcard tests/stm32vl_discovery/tsleep/*.c)
LIBS.tsleep$E = hardware$L spld$L CMSIS$L
