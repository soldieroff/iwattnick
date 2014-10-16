LIBS += CMSIS
DESCRIPTION.CMSIS = CMSIS Cortex-M3 Core Peripheral Access Layer library
DIR.INCLUDE.C += :include/CMSIS

# Define the macro for CMSIS & clients to know which instruction set is available
CMSIS.DEF.cortex-m0 = -DARM_MATH_CM0
CMSIS.DEF.cortex-m0plus = -DARM_MATH_CM0PLUS
CMSIS.DEF.cortex-m3 = -DARM_MATH_CM3
CMSIS.DEF.cortex-m4 = -DARM_MATH_CM4

CFLAGS.DEF += $($(CMSIS.DEF.$(CORE)))

DEVFAM := $(patsubst %x,%,$(patsubst %x,%,$(patsubst system_%.c,%,$(notdir $(wildcard libs/CMSIS/device/system_*.c)))))
DEVSYS := $(wildcard $(foreach x,$(DEVFAM),$(if $(findstring $x,$(CPU)),libs/CMSIS/device/system_$x*.c)))

ifeq ($(DEVSYS),)
$(error No initialization code for this device in libs/CMSIS/device/)
endif

STARTUP := $(wildcard libs/CMSIS/device/startup/startup_$(call asciidown,$(CPUFAM)).S)
ifeq ($(STARTUP),)
$(error Startup code for CPU family $(CPUFAM) not found)
endif

TARGETS.CMSIS = CMSIS$L
SRC.CMSIS$L = $(DEVSYS) $(STARTUP) $(wildcard libs/CMSIS/dsp/*/*.c)
