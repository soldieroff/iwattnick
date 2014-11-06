/*
    Platform-dependent MudBus driver
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus-stm32.h"
#include "dma.h"

void mbd_tx_mute (mudbus_driver_t *mbd, bool mute)
{
    unsigned shift = (mbd->gpio_tx_bit & 7) * 4;
    uint32_t mask = (GPIO_CNF_MASK | GPIO_MODE_MASK) << shift;
    uint32_t bits = (mute ?
        (GPIO_MODE_INPUT | GPIO_CNF_PUD) :
        (GPIO_MODE_OUTPUT_2MHz | GPIO_CNF_AF_PUSHPULL)) << shift;

    uint32_t cr = *mbd->gpio_tx_cr;
    if ((cr & mask) != bits)
    {
        // Wait until the transmitter becomes idle
        while (!(mbd->usart->SR & USART_SR_TC))
            ;
        *mbd->gpio_tx_cr = (cr & ~mask) | bits;
    }
}

void mbd_tx_start (mudbus_driver_t *mbd, const uint8_t *data, uint8_t len)
{
    // Set up DMA channel for transfer
    dma_copy (mbd->dma, mbd->dma_tx_chan,
        DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_PSIZE_8 | DMA_CCR_MSIZE_8 | DMA_CCR_PL_VERYHIGH,
        (void *)data, (void *)&mbd->usart->DR, len);

    // Enable USART -> DMA transmission
    mbd->usart->CR3 |= USART_CR3_DMAT;
}

void mbd_tx_stop (mudbus_driver_t *mbd)
{
    mbd->usart->CR3 &= ~USART_CR3_DMAT;
    dma_stop (mbd->dma, mbd->dma_tx_chan);
}

void mbd_rx_stop (mudbus_driver_t *mbd)
{
    // Disable USART -> DMA transmission
    mbd->usart->CR3 &= ~USART_CR3_DMAR;

    // Stop the DMA channel
    dma_stop (mbd->dma, mbd->dma_rx_chan);
}
