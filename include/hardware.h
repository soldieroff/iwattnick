/*
    Define the layout of your board here
    Copyright (C) 2009-2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.

    Copy this file to hardware-$(HARDWARE).h and modify it
    to describe your hardware platform.
*/

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/**
 * @file hardware.h
 *      This is just a template for your board-specific header file.
 *      It is supposed to include the MCU-specific header files, and also
 *      declare the on-board hardware connections.
 */

// Leave just the line corresponding to your board MCU
//#include "stm32f0xx.h"
//#include "stm32f10x.h"
//#include "stm32f2xx.h"
//#include "stm32f30x.h"
//#include "stm32f4xx.h"

/**
 * A hardware definition is supposed to declare all the ports and other
 * resources used (timers, DMAs and so on). There are a lot of useful
 * macros in "macros.h" which can be later used to conveniently set up
 * and access every hardware feature.
 *
 * A "hardware feature" is supposed to be the minimal atomic hardware
 * unit - a LED, a button, a sensor and so on, usually connected to a
 * single MCU pin. Most macros use the following convention
 * ("HWFN" being the "hardware feature name"):
 *
 * @li HWFN_PORT defines the port name to which the feature is
 *      connected (A, B, C etc). Used by PORT(HWFN) macro.
 * @li HWFN_BIT defines the bit number to which the feature is
 *      connected (0, 1, 2 ...). Used by BIT(HWFN) macro.
 * @li HWFN_IRQ_PRIO defines the IRQ priority corresponding to
 *      this hardware feature. Used by IRQ_PRIO(HWFN) macro.
 * @li HWFN_DMA_NUM defines the DMA controller number used for
 *      the hardware feature Used by DMA_NUM(HWFN) macro.
 * @li HWFN_DMA_CHAN defines the DMA channel number used for
 *      the hardware feature. Used by DMA_CHAN(HWFN) macro.
 * @li HWFN_DMA_IRQ_PRIO defines the IRQ priority corresponding to
 *      the DMA channel associated with this hardware feature.
 *      Used by DMA_IRQ_PRIO(HWFN) macro.
 * @li HWFN_USART defines the index of the USART corresponding
 *      to this hardware feature.
 *
 * You can later use these declarations like this, for example:
 * @code
 *      // Set the bit output in port
 *      GPIO (HWFN)->BSRR = BITV (HWFN);
 *      // Same but simpler
 *      GPIO_SET (HWFN);
 *      // Get the port state
 *      if (GPIO (HWFN)->IDR & BITV (HWFN)) ...
 *      // Set up the GPIO mode for the pin
 *      *GPIO_CR (HWFN) = GPIO_CONFIGURE (*GPIO_CR (HWFN), HWFN, OUTPUT_50MHz, PUSHPULL);
 *
 *      // DMA stuff ... clear global interrupt flag for the channel
 *      DMA->IFCR = DMA_IFCR (UART1_TX, CGIF);
 *      // Set up the DMA config register for channel
 *      DMAC (UART1_TX)->CCR = DMA_CCR (UART1_TX, MINC)
 *
 *      // Set up the DMA transmit complete IRQ ...
 *      nvic_setup (DMA_IRQ (USART1_TX), IRQ_PRIO (USART1_TX_DMA));
 * @endcode
 */

// Define AHB bus frequency
//#define AHB_FREQ	24000000
// Define APB1 bus frequency
//#define APB1_FREQ	24000000
// Define APB2 bus frequency
//#define APB2_FREQ	24000000

// RTC crystal frequency, Hz
//#define RTC_FREQ	32768

#endif // __HARDWARE_H__
