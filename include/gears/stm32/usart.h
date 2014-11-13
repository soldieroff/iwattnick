/*
    STM32 Universal Serial Asynchronous Receiver/Transmitter simplified
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __USART_H__
#define __USART_H__

/**
 * @file usart.h
 *      This small library provides some functions for sending bytes
 *      via one of the USARTs available in the STM32 microcontrollers.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "useful.h"

/// 8 bit characters
#define USART_CHARBITS_8	0x00000000
/// 9 bit characters
#define USART_CHARBITS_9	0x00100000

/// 1 stop bits
#define USART_STOPBITS_1	0x00000000
/// 0.5 stop bits
#define USART_STOPBITS_0_5	0x00200000
/// 2 stop bits
#define USART_STOPBITS_2	0x00400000
/// 1.5 stop bits
#define USART_STOPBITS_1_5	0x00600000
/// The mask to select only stop bits selection from format
#define USART_STOPBITS_MASK	0x00600000
/// The amount to shift stopbits right to compute USART_CR2_STOP bitflags
#define USART_STOPBITS_LSHIFT	9		// USART_CR2_STOP=0x3000

/// No parity check/generation
#define USART_PARITY_NONE	0x00000000
/// Even parity check/generation
#define USART_PARITY_EVEN	0x01000000
/// Odd parity check/generation
#define USART_PARITY_ODD	0x01800000

/// The mask to select only parity mode selection from format
#define USART_PARITY_MASK	0x01800000
/// The amount to shift the bits right to get USART_CR1_PS & USART_CR1_PCE
#define USART_PARITY_LSHIFT	14		// USART_CR1_PS=0x0200, USART_CR1_PCE=0x0400

/// Use CTS for outbound flow control
#define USART_CTS		0x02000000
/// Signal RTS when input buffer is full
#define USART_RTS		0x04000000

/// Disable receiver
#define USART_NO_RX		0x08000000
/// Disable transmitter
#define USART_NO_TX		0x10000000

/// The mask to separate baud rate bits from format
#define USART_BAUD_MASK		0x000FFFFF	// up to 1048575 baud

/**
 * Initialize the serial transceiver.
 * It is supposed that the GPIO themselves (clocking, mode, config,
 * alternate mappings) are set up prioir to calling this function.
 * @arg usart
 *      A pointer to the USART register block to be initialized.
 * @arg bus_freq
 *      The clock on the bus used by the USART (AHB1 or AHB2).
 * @arg fmt
 *      A combination of USART_XXX bits defined above to define
 *      the baud rate and data format.
 */
extern void usart_init (USART_TypeDef *usart, uint32_t bus_freq, uint32_t fmt);

/**
 * Redirect standard C functions like printf(), puts() and so on
 * via the passed serial port. Very useful for debugging.
 * @arg usart
 *      The USART to redirect stdio through
 */
extern void stdio_init (USART_TypeDef *usart);

/**
 * Send a single byte through the serial port.
 * @arg usart
 *      The USART to send the character through
 * @arg c
 *      The character to send
 */
extern void usart_putc (USART_TypeDef *usart, uint8_t c);

/**
 * Receive a single byte through the serial port.
 * Waits until the character is ready.
 * @arg usart
 *      The USART to read the character from
 * @return
 *      The character read from the USART
 */
extern uint8_t usart_getc (USART_TypeDef *usart);

/**
 * Check if input is ready to be read from the port.
 * @arg usart
 *      The USART to check data readiness in
 * @return
 *      true if there is data in the receive buffer
 */
static inline bool usart_rx_ready (USART_TypeDef *usart)
{ return (usart->SR & USART_SR_RXNE) != 0; }

/**
 * Check if transmission is complete (both bytes in outgoing FIFO and
 * data register).
 * @arg usart
 *      The USART to check
 * @return
 *      true if transmission is complete
 */
static inline bool usart_tx_complete (USART_TypeDef *usart)
{ return (usart->SR & USART_SR_TC) != 0; }

#ifdef __cplusplus
}
#endif

#endif // __USART_H__
