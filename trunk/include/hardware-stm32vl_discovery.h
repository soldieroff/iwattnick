/*
 * STM32VL-Discovery board hardware definition
 * Copyright (C) 2014 by Andrew Zabolotny <zapparello@ya.ru>
 */

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "macros.h"
#include "stm32f10x.h"

// A green LED on port PC9
#define GLED_PORT		C
#define GLED_BIT		9

// And a blue one on PC8
#define BLED_PORT		C
#define BLED_BIT		8

// And a button on PA0
#define USRBUT_PORT		A
#define USRBUT_BIT		0

// That's all we have, folks!

#endif // __HARDWARE_H__
