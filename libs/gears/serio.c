/*
    serial I/O simplified
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "serio.h"

void serio_init (USART_TypeDef *usart, uint32_t bus_freq, uint32_t fmt)
{
    uint32_t tmp;

    // flow control bits
    tmp = usart->CR3 & ~(USART_CR3_CTSE | USART_CR3_RTSE);

    if (fmt & USART_CTS)
        tmp |= USART_CR3_CTSE;
    if (fmt & USART_RTS)
        tmp |= USART_CR3_RTSE;

    usart->CR3 = tmp;

    // stop bits
    usart->CR2 = (usart->CR2 & ~USART_CR2_STOP) |
        ((fmt & USART_STOPBITS_MASK) << USART_STOPBITS_LSHIFT);

    // enable USART, set parity mode
    tmp = (usart->CR1 &
        ~(USART_CR1_UE | USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_RE | USART_CR1_TE)) |
        ((fmt & USART_PARITY_MASK) << USART_PARITY_LSHIFT);

    // Enable receiver & transmitter, if not prohibited
    if (!(fmt & USART_NO_RX))
        tmp |= USART_CR1_RE;
    if (!(fmt & USART_NO_TX))
        tmp |= USART_CR1_TE;

    // char bits
    if (fmt & USART_CHARBITS_9)
        tmp |= USART_CR1_M;

    usart->CR1 = tmp;

    // Finally, set up the baud rate
    fmt &= USART_BAUD_MASK;

    // It looks like STM32F1 (undocumentedly) supports OVER8 mode,
    // but we don't support such high bit rates anyway, meh...

    // Won't check for zero baud rate, we aren't idiots, are we?
    usart->BRR = (bus_freq + fmt / 2) / fmt;

    // Finally, enable the USART
    usart->CR1 |= USART_CR1_UE;
}
