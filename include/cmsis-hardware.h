/*
 * CMSIS hardware-specific definitions
 * Copyright (C) 2014 by Andrew Zabolotny <zap@cobra.ru>
 *
 * Copy this file to cmsis-$(HARDWARE).h and
 * modify it to correctly specify the running environment.
 */

#ifndef __CMSIS_HARDWARE_H__
#define __CMSIS_HARDWARE_H__

// Uncomment the following to support booting from RAM on STM32F0 and STM32F1
//#define BOOTRAM

// Uncomment the following line if you need to use external SRAM mounted
// on STM3210E-EVAL board (STM32 High density and XL-density devices) as data memory
//#define DATA_IN_ExtSRAM

// Uncomment if you're using external crystal, and set the correct frequency
//#define HSE_VALUE		8000000

#ifdef HSE_VALUE
#  define SYSCLK_FREQ_HSE	HSE_VALUE
#endif

// Define APB1 bus frequency
//#define APB1_FREQ	24000000
// Define APB2 bus frequency
//#define APB2_FREQ	24000000

#endif // __CMSIS_HARDWARE_H__
