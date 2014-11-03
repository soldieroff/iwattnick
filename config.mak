# Конфигурационный файл системы автоматической сборки для данного проекта

# Режим сборки по умолчанию
MODE = release
HARDWARE ?= iwattnick

# Описание нашей аппаратной платформы
stm32.hw2cpu.iwattnick=stm32f100c8

# Пара вспомогательных функций
asciiup = $(shell echo $1 | tr a-z A-Z)
asciidown = $(shell echo $1 | tr A-Z a-z)

# Попытка автоопределения текущей ОС (по наличию переменной окружения ComSpec)
ifndef HOST
ifneq ($(if $(ComSpec),$(ComSpec),$(COMSPEC)),)
HOST = win32
else
HOST = $(call asciidown,$(shell uname -s))
endif
endif

ARCH = arm
TARGET = none-eabi

# Проверка на установленные соответствующие утилиты
MAKEDEP = $(shell which makedep 2>/dev/null)
DOXYGEN = $(shell which doxygen 2>/dev/null)

# Определим макросы для архитектуры
CFLAGS.DEF += -DARCH_$(call asciiup,$(ARCH))

# Описание дополнительного инструментария к системе автоматизированной сборки
SUBMAKEFILES += tibs/extra/arm-none-eabi-gcc.mak $(wildcard tibs/extra/tool-*.mak)

# Local user file which can be used to override some of the settings (e.g. MODE)
-include local-config.mak

# Hardware definitions for STM32 microcontroller family
include tibs/extra/stm32.mak

CFLAGS += \
    -DHARDWARE_H=\"hardware-$(HARDWARE).h\" \
    -DCMSIS_HARDWARE_H=\"cmsis-$(HARDWARE).h\"

OUT = $(OUTBASE)/$(ARCH)-$(TARGET).$(HARDWARE)/$(MODE)/
