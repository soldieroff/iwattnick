/*
    CMSIS hardware-specific definitions
    Copyright (C) 2014 by Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __CMSIS_HARDWARE_H__
#define __CMSIS_HARDWARE_H__

// Comment out to use HSI
//#define HSE_VALUE		8000000

#ifdef HSE_VALUE
#define SYSCLK_FREQ_HSE		HSE_VALUE
#endif

#endif // __CMSIS_HARDWARE_H__
