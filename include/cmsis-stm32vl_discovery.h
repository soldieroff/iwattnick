/*
 * CMSIS hardware-specific definitions
 * Copyright (C) 2014 by Andrew Zabolotny <zap@cobra.ru>
 */

#ifndef __CMSIS_HARDWARE_H__
#define __CMSIS_HARDWARE_H__

// Comment out to use HSI
//#define HSE_VALUE		8000000

#ifdef HSE_VALUE
#define SYSCLK_FREQ_HSE		HSE_VALUE
#endif

// Define APB1 bus frequency
#define APB1_FREQ	24000000
// Define APB2 bus frequency
#define APB2_FREQ	24000000

#endif // __CMSIS_HARDWARE_H__
