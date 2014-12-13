.PHONY: all compile
.SUFFIXES: .bin

OUT = out/

GOC = ./goc.py
GOCFLAGS = -I../../include/goc

TESTS = test1 test2 test3 test4

all: $(addprefix $(OUT),$(addsuffix .S,$(TESTS)))

compile: $(addprefix $(OUT),$(addsuffix .bin,$(TESTS)))

$(OUT):
	mkdir -p $(OUT)

$(OUT)%.S: %.go $(GOC) $(OUT)
	$(GOC) $(GOCFLAGS) $< -o $@ -H $(@:.S=.h)

$(OUT)%.bin: $(OUT)%.S $(OUT)
	gcc -nostartfiles -nodefaultlibs -I ../../include -o $@ $<

$(OUT)test2.S: GOCFLAGS += -Ecp1251
$(OUT)test3.S: GOCFLAGS += -Ot
$(OUT)test4.S: GOCFLAGS += -Ol
