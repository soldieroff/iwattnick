/*
 * Define the layout of your board here
 * Copyright (C) 2009 by Andrew Zabolotny <zap@cobra.ru>
 */

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

// This always comes for free
#include "macros.h"

// Leave just the line corresponding to your board MCU
//#include "stm32f0xx.h"
//#include "stm32f10x.h"
//#include "stm32f2xx.h"
//#include "stm32f30x.h"
//#include "stm32f4xx.h"

/*
 * A hardware definition is supposed to declare all the ports and other
 * resources used (timers, DMAs and so on). There are a lot of useful
 * macros in "macros.h" which can be later used to conveniently set up
 * and access every hardware feature.
 *
 * A "hardware feature" is supposed to be the minimal atomic execution
 * unit - a LED, a button, a sensor and so on. Most macros use the
 * following convention ("HWFN" being the "hardware feature name"):
 *
 * @li HWFN_PORT defines the port name to which the feature is
 *      connected (A, B, C etc).
 * @li HWFN_BIT defines the bit number to which the feature is
 *      connected (0, 1, 2 ...).
 *
 * You can later use these declarations like this, for example:
 * @code
 *      // Set the bit output in port
 *      GPIO (HWFN)->BSRR = BITV (HWFN);
 *      // Get the port state
 *      if (GPIO (HWFN)->IDR & BITV (HWFN)) ...
 *      // Set up the GPIO mode
 *      GPIO (HWFN)->CRL = GPIO_SET (GPIO (HWFN)->CRL, HWFN, OUTPUT_50MHz, PUSHPULL);
 * @endcode
 */

#endif // __HARDWARE_H__
