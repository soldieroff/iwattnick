TESTS += tlayout
DESCRIPTION.tlayout = Test the ST7567-based LCD
FLASH.TARGETS += tlayout
IHEX.TARGETS += tlayout

DIR.tlayout = tests/stm32vl_discovery/tlayout/

TARGETS.tlayout = tlayout$E
SRC.tlayout$E = tests/stm32vl_discovery/stm32vl_discovery.c \
    $(wildcard $(DIR.tlayout)*.c) $(DIR.tlayout)gfx.S
CFLAGS.tlayout += -I$(OUT) -finput-charset=utf-8 -fexec-charset=cp1251
LIBS.tlayout$E = yagl$L gears$L useful$L CMSIS$L

$(OUT)$(DIR.tlayout)main.o: $(DIR.tlayout)gfx.h

$(DIR.tlayout)gfx.S $(DIR.tlayout)gfx.h: $(DIR.tlayout)gfx.go $(wildcard include/goc/*.go)
	tools/goc/goc.py -Ol -Iinclude/goc $< -o $(DIR.tlayout)gfx.S -H $(DIR.tlayout)gfx.h
