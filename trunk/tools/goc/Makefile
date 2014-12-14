.PHONY: all bin
.SUFFIXES: .bin

OUT = out/

GOC = ./goc.py
GOCFLAGS = -I../../include/goc

CC = gcc
CFLAGS = -s -Wl,--oformat=binary -nostartfiles -nodefaultlibs -I ../../include

TESTS = test1 test2 test3 test4

all: $(OUT) $(addprefix $(OUT),$(addsuffix .S,$(TESTS)))

bin: $(OUT) $(addprefix $(OUT),$(addsuffix .bin,$(TESTS)))

$(OUT):
	mkdir -p $(OUT)

$(OUT)%.S: %.go $(GOC)
	$(GOC) $(GOCFLAGS) $< -o $@ -H $(@:.S=.h)

$(OUT)%.bin: $(OUT)%.S
	$(CC) $(CFLAGS) -o $@ $<

$(OUT)test2.S: GOCFLAGS += -Ecp1251
$(OUT)test3.S: GOCFLAGS += -Ot
$(OUT)test4.S: GOCFLAGS += -Ol
