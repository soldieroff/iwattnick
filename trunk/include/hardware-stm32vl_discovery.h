/*
    STM32VL-Discovery board hardware definition
    Copyright (C) 2014 by Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "macros.h"
#include "stm32f10x.h"

// Define AHB bus frequency
#define AHB_FREQ	24000000
// Define APB1 bus frequency
#define APB1_FREQ	24000000
// Define APB2 bus frequency
#define APB2_FREQ	24000000

// RTC crystal frequency, Hz
#define RTC_FREQ	32768

// A green LED on port PC9
#define GLED_PORT		C
#define GLED_BIT		9

// And a blue one on PC8
#define BLED_PORT		C
#define BLED_BIT		8

// And a button on PA0
#define USRBUT_PORT		A
#define USRBUT_BIT		0

// USART1 I/O ports
#define USART1_TX_PORT		A
#define USART1_TX_BIT		9
#define USART1_RX_PORT		A
#define USART1_RX_BIT		10

// USART1 DMA channels
#define USART1_TX_DMA_NUM	1
#define USART1_TX_DMA_CHAN	4
#define USART1_TX_DMA_IRQ_PRIO	0
#define USART1_RX_DMA_NUM	1
#define USART1_RX_DMA_CHAN	5
#define USART1_RX_DMA_IRQ_PRIO	0

// That's all we have, folks!

#endif // __HARDWARE_H__
