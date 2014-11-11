# tibs macros for arm-none-eabi-gcc toolkit

.SUFFIXES: .c .cpp .o .lo .a .pc .pc.in

ARM-NONE-EABI-GCC.PFX ?= arm-none-eabi-

ARM-NONE-EABI-GCC.CC ?= $(ARM-NONE-EABI-GCC.PFX)gcc -c
ARM-NONE-EABI-GCC.CFLAGS ?= -pipe -Wall -Wextra -Werror -std=gnu99 \
    $(ARM-NONE-EABI-GCC.CFLAGS.$(CORE)) \
    $(ARM-NONE-EABI-GCC.CFLAGS.$(MODE)) \
    $(ARM-NONE-EABI-GCC.CFLAGS.DEF) $(ARM-NONE-EABI-GCC.CFLAGS.INC) $(CFLAGS)
ARM-NONE-EABI-GCC.SFLAGS ?= -pipe -D__ASSEMBLY__ -D__ASSEMBLER__ \
    $(ARM-NONE-EABI-GCC.CFLAGS.$(CORE)) \
    $(ARM-NONE-EABI-GCC.CFLAGS.$(MODE)) \
    $(ARM-NONE-EABI-GCC.CFLAGS.DEF) $(ARM-NONE-EABI-GCC.CFLAGS.INC) $(CFLAGS)
ARM-NONE-EABI-GCC.CFLAGS.DEF = $(CFLAGS.DEF) -ftrack-macro-expansion=0
ARM-NONE-EABI-GCC.CFLAGS.INC = $(if $(DIR.INCLUDE.C),-I$(subst :, -I,$(DIR.INCLUDE.C)))

# You might want to use -Os instead of -O2, depending on your priorities
ARM-NONE-EABI-GCC.CFLAGS.release ?= -g -O2 -fno-reorder-blocks \
    -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs \
    -fno-unroll-loops -ffast-math -ftree-vectorize
ARM-NONE-EABI-GCC.CFLAGS.debug ?= -g -Og -D__DEBUG__ \
    -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs \
    -fno-unroll-loops -ffast-math -ftree-vectorize

ARM-NONE-EABI-GCC.CFLAGS.cortex-m0 = -mcpu=cortex-m0 -mthumb
ARM-NONE-EABI-GCC.CFLAGS.cortex-m0plus =-mcpu=cortex-m0plus -mthumb
ARM-NONE-EABI-GCC.CFLAGS.cortex-m3 = -mcpu=cortex-m3 -mthumb
ARM-NONE-EABI-GCC.CFLAGS.cortex-m4 = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

ARM-NONE-EABI-GCC.CXX.OK := $(shell which $(ARM-NONE-EABI-GCC.PFX)g++ 2>/dev/null)

ifneq ($(ARM-NONE-EABI-GCC.CXX.OK),)
ARM-NONE-EABI-GCC.CXX ?= $(ARM-NONE-EABI-GCC.PFX)g++ -c
ARM-NONE-EABI-GCC.CXXFLAGS ?= $(ARM-NONE-EABI-GCC.CFLAGS) $(CXXFLAGS) -fno-exceptions -fno-rtti
else
ARM-NONE-EABI-GCC.CXX ?= echo "C++ compiler is not installed"; false
endif

ARM-NONE-EABI-GCC.CPP ?= $(ARM-NONE-EABI-GCC.PFX)gcc -E
ARM-NONE-EABI-GCC.CPPFLAGS ?= -pipe -x c-header $(ARM-NONE-EABI-GCC.CFLAGS.DEF) $(ARM-NONE-EABI-GCC.CFLAGS.INC)

ARM-NONE-EABI-GCC.LD ?= $(ARM-NONE-EABI-GCC.PFX)gcc
ARM-NONE-EABI-GCC.LDFLAGS ?= -pipe $(ARM-NONE-EABI-GCC.CFLAGS.$(CORE)) \
    -nostartfiles -Wl,--gc-sections -mabi=aapcs \
    $(ARM-NONE-EABI-GCC.LDFLAGS.$(MODE)) $(LDFLAGS)
ARM-NONE-EABI-GCC.LDFLAGS.LIBS ?= $(LDLIBS)

ARM-NONE-EABI-GCC.LDFLAGS.release ?= -g
ARM-NONE-EABI-GCC.LDFLAGS.debug ?= -g

ARM-NONE-EABI-GCC.LINKLIB = $(if $(findstring $L,$1),,$(if $(findstring /,$1),$1,-l$1))

ARM-NONE-EABI-GCC.MDEP ?= $(or $(MAKEDEP),makedep)
ARM-NONE-EABI-GCC.MDEPFLAGS ?= -c -a -p'$$(OUT)' $(ARM-NONE-EABI-GCC.CFLAGS.DEF) $(ARM-NONE-EABI-GCC.CFLAGS.INC) \
    $(MDEPFLAGS) -D__OPTIMIZE__

ARM-NONE-EABI-GCC.AR ?= $(ARM-NONE-EABI-GCC.PFX)ar
ARM-NONE-EABI-GCC.ARFLAGS ?= crs

# Arbitrary assumptions about the programmer options
# Most likely user will have to override this
ARM-NONE-EABI-GCC.FLASHFLAGS ?= $(STM32.FLASH.ORIGIN)

ARM-NONE-EABI-GCC.OBJCOPY ?= $(ARM-NONE-EABI-GCC.PFX)objcopy
ARM-NONE-EABI-GCC.OCFLAGS ?= -j .isr_vector -j .text -j .ARM.extab -j .preinit_array -j .init_array -j .fini_array -j .data

# Translate application/library pseudo-name into an actual file name
XFNAME.ARM-NONE-EABI-GCC = $(addprefix $$(OUT),\
    $(patsubst %$E,%$(_EX),\
    $(if $(findstring $L,$1),$(addprefix lib,$(patsubst %$L,%.a,$1)),$1)\
))

MKDEPS.ARM-NONE-EABI-GCC = \
    $(patsubst %.c,%.o,\
    $(patsubst %.cpp,%.o,\
    $(patsubst %.asm,%.o,\
    $(patsubst %.S,%.o,\
    $(call MKDEPS.DEFAULT,$1)))))

COMPILE.ARM-NONE-EABI-GCC.CXX  = $(ARM-NONE-EABI-GCC.CXX) -o $@ $(strip $(ARM-NONE-EABI-GCC.CXXFLAGS) $1) $<
COMPILE.ARM-NONE-EABI-GCC.CC   = $(ARM-NONE-EABI-GCC.CC) -o $@ $(strip $(ARM-NONE-EABI-GCC.CFLAGS) $1) $<
COMPILE.ARM-NONE-EABI-GCC.S    = $(ARM-NONE-EABI-GCC.CC) -o $@ $(strip $(ARM-NONE-EABI-GCC.SFLAGS) $1) $<

# Compilation rules ($1 = source file list, $2 = source file directories,
# $3 = module name, $4 = target name)
define MKCRULES.ARM-NONE-EABI-GCC
$(if $(filter %.c,$1),$(foreach z,$2,
$(addsuffix %.o,$(addprefix $$(OUT),$z)): $(addsuffix %.c,$z)
	$(if $V,,@echo COMPILE.ARM-NONE-EABI-GCC.CC $$< &&)$$(call COMPILE.ARM-NONE-EABI-GCC.CC,$(CFLAGS.$3) $(CFLAGS.$4) $(call .LIBFLAGS,CFLAGS,$3,$4))))
$(if $(filter %.cpp,$1),$(foreach z,$2,
$(addsuffix %.o,$(addprefix $$(OUT),$z)): $(addsuffix %.cpp,$z)
	$(if $V,,@echo COMPILE.ARM-NONE-EABI-GCC.CXX $$< &&)$$(call COMPILE.ARM-NONE-EABI-GCC.CXX,$(CXXFLAGS.$3) $(CXXFLAGS.$4) $(call .LIBFLAGS,CFLAGS,$3,$4))))
$(if $(filter %.S,$1),$(foreach z,$2,
$(addsuffix %.o,$(addprefix $$(OUT),$z)): $(addsuffix %.S,$z)
	$(if $V,,@echo COMPILE.ARM-NONE-EABI-GCC.S $$< &&)$$(call COMPILE.ARM-NONE-EABI-GCC.S,$(CFLAGS.$3) $(CFLAGS.$4) $(call .LIBFLAGS,CFLAGS,$3,$4))))
$(ARM-NONE-EABI-GCC.EXTRA.MKCRULES)
endef

LINK.ARM-NONE-EABI-GCC.AR = $(ARM-NONE-EABI-GCC.AR) $(ARM-NONE-EABI-GCC.ARFLAGS) $@ $^
define LINK.ARM-NONE-EABI-GCC.EXEC
    $(ARM-NONE-EABI-GCC.LD) -o $@ $(ARM-NONE-EABI-GCC.LDFLAGS) $(LDFLAGS) $1 $(filter-out %.ld,$^) \
        $(if $(filter %.ld,$^),-Wl$(COMMA)-T$(filter %.ld,$^)) \
        $(ARM-NONE-EABI-GCC.LDFLAGS.LIBS) $(LDFLAGS.LIBS) $2 -Wl,-Map,$@.map
    size $@
endef

# Linking rules ($1 = target full filename, $2 = dependency list,
# $3 = module name, $4 = unexpanded target name)
define MKLRULES.ARM-NONE-EABI-GCC
$1: $2\
$(if $(findstring $L,$4),
	$(if $V,,@echo LINK.ARM-NONE-EABI-GCC.AR $$@ &&)$$(LINK.ARM-NONE-EABI-GCC.AR))\
$(if $(findstring $E,$4), $(ARM-NONE-EABI-GCC.LDSCRIPT)
	$(if $V,,@echo LINK.ARM-NONE-EABI-GCC.EXEC $$@ &&)$$(call LINK.ARM-NONE-EABI-GCC.EXEC,$(subst $(COMMA),$$(COMMA),$(LDFLAGS.$3) $(LDFLAGS.$4)) $(call .LIBFLAGS,LDLIBS,$3,$4),$(foreach z,$(LIBS.$3) $(LIBS.$4),$(call ARM-NONE-EABI-GCC.LINKLIB,$z))))
$(ARM-NONE-EABI-GCC.EXTRA.MKLRULES)
endef

# Install rules ($1 = module name, $2 = unexpanded target file,
# $3 = full target file name)
define MKIRULES.ARM-NONE-EABI-GCC
$(if $(findstring $L,$2),\
$(foreach _,$3,
	$(if $V,,@echo INSTALL $_ to $(call .INSTDIR,$1,$2,LIB,$(CONF_LIBDIR)) &&)\
	$$(call INSTALL,$_,$(call .INSTDIR,$1,$2,LIB,$(CONF_LIBDIR)),0644)))\
$(if $(findstring $E,$2),
	$(if $V,,@echo INSTALL $3 to $(call .INSTDIR,$1,$2,BIN,$(CONF_BINDIR)) &&)\
	$$(call INSTALL,$3,$(call .INSTDIR,$1,$2,BIN,$(CONF_BINDIR)),0755))\
$(if $(INSTALL.INCLUDE.$2),
	$(if $V,,@echo INSTALL $(INSTALL.INCLUDE.$2) to $(call .INSTDIR,$1,$2,INCLUDE,$(CONF_INCLUDEDIR)) &&)\
	$$(call INSTALL,$(INSTALL.INCLUDE.$2),$(call .INSTDIR,$1,$2,INCLUDE,$(CONF_INCLUDEDIR)),0644))
endef

define MAKEDEP.ARM-NONE-EABI-GCC
	$(call RM,$@)
	$(if $(filter %.c,$^),$(ARM-NONE-EABI-GCC.MDEP) $(strip $(ARM-NONE-EABI-GCC.MDEPFLAGS) $(filter -D%,$1) $(filter -I%,$1)) -f $@ $(filter %.c,$^))
	$(if $(filter %.cpp,$^),$(ARM-NONE-EABI-GCC.MDEP) $(strip $(ARM-NONE-EABI-GCC.MDEPFLAGS) $(filter -D%,$2) $(filter -I%,$2)) -f $@ $(filter %.cpp,$^))
	$(if $(filter %.S,$^),$(ARM-NONE-EABI-GCC.MDEP) $(strip $(ARM-NONE-EABI-GCC.MDEPFLAGS) $(filter -D%,$3) $(filter -I%,$3)) -f $@ $(filter %.S,$^))
endef

# Dependency rules ($1 = dependency file, $2 = source file list,
# $3 = module name, $4 = target name)
define MKDRULES.ARM-NONE-EABI-GCC
$1: $(MAKEDEP_DEP) $2
	$(if $V,,@echo MAKEDEP.ARM-NONE-EABI-GCC $$@ &&)$$(call MAKEDEP.ARM-NONE-EABI-GCC,\
	$(subst $(COMMA),$$(COMMA),$(CFLAGS.$3) $(CFLAGS.$4) $(CFLAGS)),\
	$(subst $(COMMA),$$(COMMA),-D__cplusplus $(CXXFLAGS.$3) $(CXXFLAGS.$4) $(CXXFLAGS)),\
	$(subst $(COMMA),$$(COMMA),$(SFLAGS.$3) $(SFLAGS.$4) $(SFLAGS)))
$(ARM-NONE-EABI-GCC.EXTRA.MKDRULES)
endef

ARM-NONE-EABI-GCC.FLASH=st-flash write $1 $(if $2,$2,$(ARM-NONE-EABI-GCC.FLASHFLAGS))

# Flashing rules ($1 = module name, $2 = unexpanded target file,
# $3 = full target file name)
define MKFRULES.ARM-NONE-EABI-GCC
$(if $(filter %$E,$2),
	$(if $V,,@echo FLASH.ARM-NONE-EABI-GCC $3 &&)\
	$$(call ARM-NONE-EABI-GCC.FLASH,$3,$(strip $(FLASHFLAGS.$1) $(FLASHFLAGS.$2))))
endef

LINK.ARM-NONE-EABI-GCC.HEX = $(ARM-NONE-EABI-GCC.OBJCOPY) $< $(ARM-NONE-EABI-GCC.OCFLAGS) $1 -O ihex $@

# iHEXing rules ($1 = module name, $2 = unexpanded target file,
# $3 = full target file name, $4 = executable name)
define MKIXRULES.ARM-NONE-EABI-GCC
$(if $(findstring $E,$2),
$3: $4
	$(if $V,,@echo LINK.ARM-NONE-EABI-GCC.HEX $$@ &&)$$(call LINK.ARM-NONE-EABI-GCC.HEX,$(OCFLAGS.$3) $(OCFLAGS.$4)))
endef

LINK.ARM-NONE-EABI-GCC.BIN ?= $(ARM-NONE-EABI-GCC.OBJCOPY) $< $(ARM-NONE-EABI-GCC.OCFLAGS) $1 -O binary $@

# Rules to build binary images ($1 = module name, $2 = unexpanded target file,
# $3 = full target file name, $4 = executable name)
define MKBINRULES.ARM-NONE-EABI-GCC
$(if $(findstring $E,$2),
$3: $4
	$(if $V,,@echo LINK.ARM-NONE-EABI-GCC.BIN $$@ &&)\
	$$(call LINK.ARM-NONE-EABI-GCC.BIN,$(OCFLAGS.$3) $(OCFLAGS.$4),$(strip $(FLASHFLAGS.$1) $(FLASHFLAGS.$2))))
endef

# Это должен знать каждый!
ARM-NONE-EABI-GCC.CFLAGS.DEF += \
	-DSTM32_STACK_ADDRESS=$(STM32.RAM.END) \
	-DSTM32_MIN_HEAP_SIZE=$(STM32_MIN_HEAP_SIZE) \
	-DSTM32_MIN_STACK_SIZE=$(STM32_MIN_STACK_SIZE) \
	-DSTM32_FLASH_ORIGIN=$(STM32.FLASH.ORIGIN) \
	-DSTM32_FLASH_SIZE=$(STM32.FLASH.SIZE) \
	-DSTM32_RAM_ORIGIN=$(STM32.RAM.ORIGIN) \
	-DSTM32_RAM_SIZE=$(STM32.RAM.SIZE)

# Правила генерации скрипта для линкера
ARM-NONE-EABI-GCC.LDSCRIPT = $(OUT)stm32_flash.ld

$(ARM-NONE-EABI-GCC.LDSCRIPT): tibs/extra/stm32_flash.ld.in
	$(if $V,,@echo ARM-NONE-EABI-GCC.CPP $@ &&)$(ARM-NONE-EABI-GCC.CPP) \
		$(ARM-NONE-EABI-GCC.CPPFLAGS) -P -o $@ $<
