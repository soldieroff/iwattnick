TESTS += trtc
DESCRIPTION.trtc = Test for Real-Time-Clock library
FLASH.TARGETS += trtc
IHEX.TARGETS += trtc

TARGETS.trtc = trtc$E
SRC.trtc$E = tests/stm32vl_discovery/stm32vl_discovery.c $(wildcard tests/stm32vl_discovery/trtc/*.c)
LIBS.trtc$E = CMSIS$L gears$L useful$L
