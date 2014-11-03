/*
    Platform-dependent library for MudBus
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __MUDBUS_STM32_H__
#define __MUDBUS_STM32_H__

/**
 * @file mudbus-stm32.h
 *      This file defines the platform-specific functions
 *      for the MudBus library on STM32 microcontroller family.
 */

// MudBus Master USART TX hardware feature
#define MBM_USART_TX		JOIN3 (USART, MBM_USART, _TX)
// MudBus Master USART RX hardware feature
#define MBM_USART_RX		JOIN3 (USART, MBM_USART, _RX)

// MudBus Slave USART TX hardware feature
#define MBS_USART_TX		JOIN3 (USART, MBS_USART, _TX)
// MudBus Slave USART RX hardware feature
#define MBS_USART_RX		JOIN3 (USART, MBS_USART, _RX)

/// Platform-specific structure that represents the USART
typedef USART_TypeDef mudbus_usart_t;

/**
 * Initialize MudBus master USART transceiver.
 */
extern void mbm_usart_init ();

/**
 * Mute the output on TX pin. When TX is muted, the USART output does
 * not change TX pin state, otherwise it works as usual. This is a very
 * handy mode to count off timeouts defined in USART character frames.
 *
 * You must disable mute to return into normal mode.
 */
extern void mbm_usart_tx_mute (bool mute);

#endif // __MUDBUS_STM32_H__
