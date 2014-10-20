TESTS += tflash
DESCRIPTION.tflash = Flash memory self-programming demo
FLASH.TARGETS += tflash

TARGETS.tflash = tflash$E
CFLAGS.tflash += -DUSE_STDPERIPH_DRIVER -Itests/stm32vl_discovery/tflash
SRC.tflash$E = $(wildcard tests/stm32vl_discovery/tflash/*.c)
LIBS.tflash$E = hardware$L spld$L CMSIS$L
