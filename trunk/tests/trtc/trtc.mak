TESTS += trtc
DESCRIPTION.trtc = Test for Real-Time-Clock library
FLASH.TARGETS += trtc
IHEX.TARGETS += trtc

TARGETS.trtc = trtc$E
SRC.trtc$E = $(wildcard tests/trtc/*.c)
LIBS.trtc$E = CMSIS$L
