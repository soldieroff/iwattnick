# Название проекта и версия
CONF_PACKAGE = iwattnick
CONF_VERSION = 0.0.1

# Include the generated by configure config.mak
-include config.mak

# Check if the project has been configured
ifndef TARGET
$(error You must run configure before starting compiling)
endif

# The directory where tibs makefiles are located
DIR.TIBS ?= tibs

# Default build mode
MODE ?= release

# No automatic .dep building (only explicitly by make dep)
AUTODEP ?= 0

# Extra files to include in distribution
DISTEXTRA += include/ libs/ tibs/ config.mak local-config.mak

# Default toolkit
ifeq ($(ARCH),arm)
TOOLKIT ?= ARM-NONE-EABI-GCC
else
TOOLKIT ?= GCC
endif

# Build tools if they aren't available
ifndef MAKEDEP
MAKEDEP = $(OUT)makedep$(EXE)
MAKEDEP_DEP = $(MAKEDEP)
endif

include $(DIR.TIBS)/rules.mak
# These rules must always come last
include \
    tibs/extra/flash-rules.mak \
    tibs/extra/ihex-rules.mak \
    tibs/extra/bin-rules.mak

showhelp::
	$(call SAY,Target hardware: $(HARDWARE)$(COMMA) cpu: $(CPU)$(COMMA) core: $(CORE))
	$(call SAY,$-)
