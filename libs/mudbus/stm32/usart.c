/*
    Platform-dependent USART library for MudBus
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus-stm32.h"
#include "usart.h"

// Some magic with preprocessor to support any USARTs on any ports

// Enable clock to port A/B/C/... depending where our UART is connected to
#define MBM_APB2ENR_BITS_1	JOIN3 (RCC_APB2ENR_IOP, PORT (JOIN3 (USART, MBM_USART, _TX)), EN)

// USART1 enables via APB2ENR, other USARTs - via APB1ENR
#if USART_CMP (MBM, 1)
#  define MBM_APB1ENR_BITS_2	0
#  define MBM_APB2ENR_BITS_2	JOIN3 (RCC_APB2ENR_USART, MBM_USART, EN)
#  define MBM_USART_BUS_FREQ	APB2_FREQ
#else
#  define MBM_APB1ENR_BITS_2	JOIN3 (RCC_APB1ENR_USART, MBM_USART, EN)
#  define MBM_APB2ENR_BITS_2	0
#  define MBM_USART_BUS_FREQ	APB1_FREQ
#endif

void mbm_usart_init ()
{
    // Enable clocking GPIO, USART and AFIO
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | MBM_APB2ENR_BITS_1 | MBM_APB2ENR_BITS_2;
    RCC->APB1ENR |= MBM_APB1ENR_BITS_2;

    // Initialize RX and TX
    mbm_usart_tx_mute (false);

    *GPIO_CR (MBM_USART_RX) = GPIO_SET (
        *GPIO_CR (MBM_USART_RX), MBM_USART_RX, INPUT, FLOATING);

    // Initialize the USART
    usart_init (USART (MBM), MBM_USART_BUS_FREQ, MBM_USART_SETUP);
}

void mbm_usart_tx_mute (bool mute)
{
    if (mute)
    {
        // output with weak pull-up
        *GPIO_CR (MBM_USART_TX) = GPIO_SET (
            *GPIO_CR (MBM_USART_TX), MBM_USART_TX, OUTPUT_2MHz, PUD);
        GPIO_BSET (MBM_USART_TX);
    }
    else
        // alternative function push-pull
        *GPIO_CR (MBM_USART_TX) = GPIO_SET (
            *GPIO_CR (MBM_USART_TX), MBM_USART_TX, OUTPUT_2MHz, AF_PUSHPULL);
}
