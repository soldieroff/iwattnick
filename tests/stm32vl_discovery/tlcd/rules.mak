TESTS += tlcd
DESCRIPTION.tlcd = Test the ST7567-based LCD
FLASH.TARGETS += tlcd
IHEX.TARGETS += tlcd

TARGETS.tlcd = tlcd$E
SRC.tlcd$E = tests/stm32vl_discovery/stm32vl_discovery.c \
    $(wildcard tests/stm32vl_discovery/tlcd/*.c) \
    $(OUT)gfx.S
CFLAGS.tlcd += -I$(OUT) -finput-charset=utf-8 -fexec-charset=cp1251
LIBS.tlcd$E = yagl$L gears$L useful$L CMSIS$L

$(OUT)tests/stm32vl_discovery/tlcd/main.o: $(OUT)gfx.h

$(OUT)gfx.S $(OUT)gfx.h: tests/stm32vl_discovery/tlcd/gfx.go
	tools/goc/goc.py -Iinclude/goc $< -o $(OUT)gfx.S -H $(OUT)gfx.h
