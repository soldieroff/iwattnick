TESTS += iwattnick
DESCRIPTION.iwattnick = iWattnick the on-board computer for e-transport
FLASH.TARGETS += iwattnick
IHEX.TARGETS += iwattnick

DIR.iwattnick = apps/iwattnick/
TARGETS.iwattnick = iwattnick$E
SRC.iwattnick$E = $(wildcard $(DIR.iwattnick)*.c) $(DIR.iwattnick)igo.S
LIBS.iwattnick$E = CMSIS$L mudbus$L gears$L useful$L

$(OUT)$(DIR.iwattnick)main.o: $(DIR.iwattnick)igo.h

$(DIR.iwattnick)igo.S $(DIR.iwattnick)igo.h: $(wildcard $(DIR.iwattnick)*.go) $(wildcard include/goc/*.go)
	tools/goc/goc.py -Olm -Iinclude/goc $< -o $(DIR.iwattnick)igo.S -H $(DIR.iwattnick)igo.h
