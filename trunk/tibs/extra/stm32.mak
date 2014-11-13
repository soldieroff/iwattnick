# STM32 compiler definitions

include tibs/extra/stm32-defs.mak

# Определим тип процессора для нашей аппаратной платформы, а также тип ядра процессора
MCU = stm32
CPU = $(STM32.HW2CPU.$(HARDWARE))
CORE = $(word 1,$(STM32.CPUDEF.$(CPU)))

# Решим к какому семейству относится наш процессор
# Классификация приведена в include/CMSIS/stm32f*.h
STM32.CPUFAM = STM32F10X_LD STM32F10X_LD_VL STM32F10X_MD STM32F10X_MD_VL STM32F10X_HD STM32F10X_XL STM32F10X_CL
STM32.CPUFAM.STM32F10X_LD = stm32f101%4 stm32f101%6 stm32f102%4 stm32f102%6 stm32f103%4 stm32f103%6
STM32.CPUFAM.STM32F10X_LD_VL = stm32f100%4 stm32f100%6
STM32.CPUFAM.STM32F10X_MD = stm32f101%8 stm32f101%b stm32f102%8 stm32f102%b stm32f103%8 stm32f103%b
STM32.CPUFAM.STM32F10X_MD_VL = stm32f100%8 stm32f100%b
STM32.CPUFAM.STM32F10X_HD = stm32f10%c stm32f10%d stm32f10%e
STM32.CPUFAM.STM32F10X_XL = stm32f10%f stm32f10%g
STM32.CPUFAM.STM32F10X_CL = stm32f105% stm32f107%

CPUFAM = $(strip $(foreach x,$(STM32.CPUFAM),$(if $(strip $(filter $(STM32.CPUFAM.$x),$(CPU))),$x)))
ifeq ($(CPUFAM),)
$(error Cannot determine the family for this CPU type)
endif

STM32.MEM = $(shell python tibs/extra/stm32-mem.py $(CPU) $(CPUFAM) $(word 2,$(STM32.CPUDEF.$(CPU))) $(word 3,$(STM32.CPUDEF.$(CPU))))
ifeq ($(STM32.MEM),)
$(error Failed to determine memory origin & sizes for CPU $(CPU))
endif

STM32.FLASH.ORIGIN = $(word 1,$(STM32.MEM))
STM32.FLASH.SIZE = $(word 2,$(STM32.MEM))
STM32.FLASH.END = $(word 3,$(STM32.MEM))
STM32.RAM.ORIGIN = $(word 4,$(STM32.MEM))
STM32.RAM.SIZE = $(word 5,$(STM32.MEM))
STM32.RAM.END = $(word 6,$(STM32.MEM))
STM32.FAMILY = $(word 7,$(STM32.MEM))

# User may override these, if needed
STM32_MIN_HEAP_SIZE ?= 0
STM32_MIN_STACK_SIZE ?= 0x200

# Набор инструкций для библиотеки CMSIS/arm_math.h
MATH.cortex-m0 = ARM_MATH_CM0
MATH.cortex-m0plus = ARM_MATH_CM0PLUS
MATH.cortex-m3 = ARM_MATH_CM3
MATH.cortex-m4 = ARM_MATH_CM4

# Расскажем о типе процессора программе
CFLAGS.DEF += -DMCU_$(call asciiup,$(MCU)) -DCPU_$(call asciiup,$(CPU)) \
    -DCORE_$(call asciiup,$(subst -,_,$(CORE))) -D$(CPUFAM) -D$(MATH.$(CORE))
