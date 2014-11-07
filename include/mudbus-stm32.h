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

/**
 * The MudBus "driver" which contains all the data to deal with
 * underlying hardware.
 */
typedef struct
{
    /// A pointer to USART object
    USART_TypeDef *usart;
    /// The DMA controller associated with the USART
    DMA_TypeDef *dma;
    /// A pointer to USART->CRL/CRH for RX/TX
    volatile uint32_t *gpio_tx_cr;
    /// GPIO bit number corresponding to TX
    uint8_t gpio_tx_bit;
    /// The DMA channel that works with UART TX
    uint8_t dma_tx_chan;
    /// The DMA channel that works with UART RX
    uint8_t dma_rx_chan;
} mudbus_driver_t;

/**
 * Initialize MudBus Master low-level driver.
 */
extern void mbd_master_init (mudbus_driver_t *mbd);

/**
 * Mute the output on TX pin. When TX is muted, the USART output does
 * not change TX pin state, otherwise it works as usual. This is a very
 * handy mode to count off timeouts defined in USART character frames.
 *
 * You must disable mute to return into normal mode.
 * @arg mbd
 *      The platform-dependent driver
 * @arg mute
 *      true to mute TX output, false to enable it back
 */
extern void mbd_tx_mute (mudbus_driver_t *mbd, bool mute);

/**
 * Check if USART transmitter is still busy sending data.
 * In fact, this checks if USART TX is connected to DMA
 * since the DMAT flag in USART is dropped in interrupt.
 * @arg mbd
 *      The platform-dependent driver
 * @return
 *      true if USART TX is busy
 */
static inline bool mbd_tx_busy (mudbus_driver_t *mbd)
{ return (mbd->usart->CR3 & USART_CR3_DMAT) != 0; }

/**
 * Start packet fragment transmission using the driver.
 * @arg mbd
 *      The platform-dependent driver
 * @arg data
 *      A pointer to packet fragment data
 * @arg len
 *      The length of the packet fragment
 */
extern void mbd_tx_start (mudbus_driver_t *mbd, const uint8_t *data, uint8_t len);

/**
 * Stop the USART transmitter and the DMA controller.
 * @arg mbd
 *      The platform-dependent driver
 */
extern void mbd_tx_stop (mudbus_driver_t *mbd);

/**
 * Stop the USART receiver and the DMA controller.
 * @arg mbd
 *      The platform-dependent driver
 */
extern void mbd_rx_stop (mudbus_driver_t *mbd);

/**
 * Check if USART receiver is still busy receiving data.
 * In fact, this checks if USART RX is connected to DMA
 * since the DMAR flag in USART is dropped in interrupt.
 * @arg mbd
 *      The platform-dependent driver
 * @return
 *      true if USART RX is busy
 */
static inline bool mbd_rx_busy (mudbus_driver_t *mbd)
{ return (mbd->usart->CR3 & USART_CR3_DMAR) != 0; }

#endif // __MUDBUS_STM32_H__
