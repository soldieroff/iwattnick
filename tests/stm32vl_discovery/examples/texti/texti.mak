TESTS += texti
DESCRIPTION.texti = External interrupt line demo
FLASH.TARGETS += texti
IHEX.TARGETS += texti

TARGETS.texti = texti$E
CFLAGS.texti += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/examples/texti
SRC.texti$E = $(wildcard tests/stm32vl_discovery/examples/texti/*.c)
LIBS.texti$E = hardware$L spld$L CMSIS$L
