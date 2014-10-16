/*
 * Define the layout of your board here
 * Copyright (C) 2009 by Andrew Zabolotny <zap@cobra.ru>
 */

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#if defined CORE_CORTEX_M4
#  define ARM_MATH_CM4
#elif defined CORE_CORTEX_M3
#  define ARM_MATH_CM3
#elif defined CORE_CORTEX_M0PLUS
#  define ARM_MATH_CM0PLUS
#elif defined CORE_CORTEX_M0
#  define ARM_MATH_CM0
#else
#  error "Unknown CPU core"
#endif

// Uncomment to use external crystal
//#define SYSCLK_FREQ_HSE

#endif // __HARDWARE_H__
