LIBS += yagl
DESCRIPTION.yagl = Yet Another Graphics Library
TARGETS.yagl = yagl$L

SRC.yagl$L := $(wildcard libs/yagl/*.c) $(wildcard libs/yagl/$(MCU)/*.c)
LIBS.yagl$L = CMSIS$L gears$L useful$L
CFLAGS.yagl$L = -Iinclude/yagl
