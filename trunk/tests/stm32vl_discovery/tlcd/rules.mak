TESTS += tlcd
DESCRIPTION.tlcd = Test the ST7567-based LCD
FLASH.TARGETS += tlcd
IHEX.TARGETS += tlcd

DIR.tlcd = tests/stm32vl_discovery/tlcd/

TARGETS.tlcd = tlcd$E
SRC.tlcd$E = tests/stm32vl_discovery/stm32vl_discovery.c \
    $(wildcard $(DIR.tlcd)*.c) $(DIR.tlcd)gfx.S
CFLAGS.tlcd += -I$(OUT) -finput-charset=utf-8 -fexec-charset=cp1251
LIBS.tlcd$E = yagl$L gears$L useful$L CMSIS$L

$(OUT)$(DIR.tlcd)main.o: $(DIR.tlcd)gfx.h

$(DIR.tlcd)gfx.S $(DIR.tlcd)gfx.h: $(DIR.tlcd)gfx.go
	tools/goc/goc.py -Ot -Iinclude/goc $< -o $(DIR.tlcd)gfx.S -H $(DIR.tlcd)gfx.h
