# This directory contains sample code for the STM32VL-DISCOVERY board

ifeq ($(HARDWARE),stm32vl_discovery)
DIR.INCLUDE.C += :tests/stm32vl_discovery
include $(wildcard tests/stm32vl_discovery/*/*.mak)
ifdef STOCK_EXAMPLES
include $(wildcard tests/stm32vl_discovery/examples/*/*.mak)
endif
endif
