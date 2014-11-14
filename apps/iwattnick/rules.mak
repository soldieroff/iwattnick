TESTS += iwattnick
DESCRIPTION.iwattnick = iWattnick the on-board computer for e-transport
FLASH.TARGETS += iwattnick
IHEX.TARGETS += iwattnick

TARGETS.iwattnick = iwattnick$E
SRC.iwattnick$E = $(wildcard apps/iwattnick/*.c)
LIBS.iwattnick$E = CMSIS$L mudbus$L gears$L useful$L
