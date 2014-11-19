/*
    STM32VL-Discovery board hardware definition
    Copyright (C) 2014 by Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

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

// Define the USART to use with MudBus in master mode
#define MBM_USART		1
// Define the USART to use with MudBus in slave mode
#define MBS_USART		2

// Serial port setup for MudBus Master
#define USART1_SETUP		(9600 | USART_CHARBITS_8 | USART_PARITY_NONE | USART_STOPBITS_1)
// Serial port setup for MudBus Slave
#define USART2_SETUP		(9600 | USART_CHARBITS_8 | USART_PARITY_NONE | USART_STOPBITS_1)

// USART IRQ priorities
#define USART1_IRQ_PRIO		((1 << __NVIC_PRIO_BITS) - 1)
#define USART2_IRQ_PRIO		((1 << __NVIC_PRIO_BITS) - 1)

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

// USART2 I/O ports
#define USART2_TX_PORT		A
#define USART2_TX_BIT		2
#define USART2_RX_PORT		A
#define USART2_RX_BIT		3

// USART2 DMA channels
#define USART2_TX_DMA_NUM	1
#define USART2_TX_DMA_CHAN	7
#define USART2_TX_DMA_IRQ_PRIO	0
#define USART2_RX_DMA_NUM	1
#define USART2_RX_DMA_CHAN	6
#define USART2_RX_DMA_IRQ_PRIO	0

// Where we have connected the LCD based on the ST7567 controlled in SPI mode
#define ST7567_SPI		1

// Chip Select /CS
#define ST7567_CS_PORT		B
#define ST7567_CS_BIT		0
// Reset /RES
#define ST7567_RES_PORT		B
#define ST7567_RES_BIT		1
// Register Select RS (A0) (0 = instruction, 1 = data)
#define ST7567_RS_PORT		B
#define ST7567_RS_BIT		2
// Serial clock SCK
#define ST7567_SCK_PORT		B
#define ST7567_SCK_BIT		3
// Serial data SDA (MOSI)
#define ST7567_SDA_PORT		B
#define ST7567_SDA_BIT		5

// That's all we have, folks!

#endif // __HARDWARE_H__
