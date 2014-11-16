.SUFFIXES: .po .pot .mo

VERSION=$(shell cat VERSION)

# Default target - do nothing
all:
	@echo "Make one of the following targets:"
	@echo "	install		install the library and tools"
	@echo "	dist		build a source code distribution"
	@echo "	win-install	build a Windows binary distribution"
	@echo "	clean		remove all generated files"
	@echo "	update-po	update all po files with new strings from sources"
	@echo "	dist		make a distribution archive"
	@echo "	msg		recompile message files in-place"

install:
	python setup.py install

win-install:
	rm -rf iwacon-$(VERSION)
	mkdir iwacon-$(VERSION)
	cp -a build/usb .
	python setup.py --4win install -O1 --root iwacon-$(VERSION)
	mv iwacon-$(VERSION)/usr/bin/iwacon iwacon-$(VERSION)/iwacon.pyw
	mv iwacon-$(VERSION)/usr/lib/python*/site-packages/iwattnick iwacon-$(VERSION)
	mv iwacon-$(VERSION)/usr/share/locale iwacon-$(VERSION)
	mv iwacon-$(VERSION)/usr/share/iwacon iwacon-$(VERSION)/share
	rm -rf iwacon-$(VERSION)/usr
	convert -background none iwacon-$(VERSION)/share/iwacon.svg -resize 64 iwacon-$(VERSION)/iwacon.ico
	zip -mr iwacon-$(VERSION)-windows.zip iwacon-$(VERSION)

dist:
	rm -rf iwacon-$(VERSION)
	mkdir -p iwacon-$(VERSION)/{po,share,iwattnick,build,debian,distr/prereq}
	cp -a po/*.po* iwacon-$(VERSION)/po
	cp -a share/*.xml share/*.svg iwacon-$(VERSION)/share
	cp -a iwattnick/*.py iwacon-$(VERSION)/iwattnick
	cp -a build/iwacon.{spec,desktop} iwacon-$(VERSION)/build
	cp -a build/usb iwacon-$(VERSION)/build
	cp -a debian/{changelog,compat,control,copyright,dirs,docs,pycompat,rules} iwacon-$(VERSION)/debian
	cp -a distr/*.html iwacon-$(VERSION)/distr
	cp -a distr/prereq/*.{html,cmd} iwacon-$(VERSION)/distr/prereq
#	cp -a docs/* iwacon-$(VERSION)/docs
	cp -a Makefile VERSION setup.py \
	      iwacon iwacon-cli iwacon-$(VERSION)
	tar cvjf iwacon-$(VERSION).tar.bz2 iwacon-$(VERSION)
	rm -rf iwacon-$(VERSION)

clean:
	python setup.py clean -a
	rm -f iwattnick/*.pyc *.log
	rm -rf iwacon-$(VERSION)

# Update translation files
update-po: $(wildcard po/*.po)

po/%.po: $(wildcard share/*.xml) $(wildcard iwattnick/*.py) iwacon
	xgettext --omit-header -L glade $(filter %.xml,$^) -o tmp.po
	xgettext --omit-header -L python $(filter-out %.xml,$^) -j -o tmp.po
	msgmerge $@ tmp.po -o $@
	rm -f tmp.po

LANGS = $(patsubst %.po,%,$(notdir $(wildcard po/*.po)))

.PHONY: msg
msg: $(patsubst %,msg-%,$(LANGS))

define MSG_RULE
.PHONY: msg-$1
msg-$1: locale/$1/LC_MESSAGES/iwacon.mo

locale/$1/LC_MESSAGES/iwacon.mo: po/$1.po
	mkdir -p $$(dir $$@)
	msgfmt $$< -o $$@

endef

$(eval $(foreach x,$(LANGS),$(call MSG_RULE,$x)))
