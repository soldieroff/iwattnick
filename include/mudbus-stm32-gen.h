/*
    Platform-dependent MudBus driver
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus-stm32.h"
#include "nvic.h"
#include "usart.h"
#include "dma.h"

/*
    @file mudbus-stm32-gen.h
    This file will generate C code for initializing the 'driver' field of a mudbus_t
    structure with information specific for a UART/DMA combination for the STM32
    platform. Also it performs actual hardware initialization.

    You must include this file from your C code, which will result in a new function
    being defined. You must call this function before mb_init.

    This file heavily depends on the MBM_XXX defines in users's hardware.h.
    Here's a example:

// Define the USART to use with MudBus in master mode
#define MBM_USART		1

// Serial port setup for MudBus Master
#define USART1_SETUP		(9600 | USART_CHARBITS_8 | USART_PARITY_NONE | USART_STOPBITS_1)

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

    Then in your C code you generate the driver code this way:

// Define the name of the generated function
#define MB_DRVINIT		mbd_init_master
// Define the USART to use for this interface
#define MB_USART		MBM_USART
// And finally, tell the code which global variable contains the MudBus instance
#define MB_VAR			mbm
#include "mudbus-stm32-gen.h"

    Then you initialize the MudBus this way:

    // MudBus Master instance
    mudbus_t mbm;
    ...
    mbd_init_master (&mbm.driver);
    mb_init (&mbm);
    ...
    // Enable DMA IRQ processing
    __enable_irq ();
*/

// Some magic with preprocessor to support any USARTs on any ports

// Shortcut for MudBus Master USART TX hardware feature
#define MB_USART_TX		JOIN3 (USART, MB_USART, _TX)
// Shortcut for MudBus Master USART RX hardware feature
#define MB_USART_RX		JOIN3 (USART, MB_USART, _RX)

// Enable clock to port A/B/C/... depending where our UART is connected to
#define MB_APB2ENR_BITS	JOIN3 (RCC_APB2ENR_IOP, PORT (JOIN3 (USART, MB_USART, _TX)), EN)

// USART1 enables via APB2ENR, other USARTs - via APB1ENR
#if USART_CMP (MB, 1)
#  define MB_APB1ENR_BITS	0
#  define MB_APB2ENR_BITS_2	JOIN3 (RCC_APB2ENR_USART, MB_USART, EN)
#  define MB_USART_BUS_FREQ	APB2_FREQ
#else
#  define MB_APB1ENR_BITS	JOIN3 (RCC_APB1ENR_USART, MB_USART, EN)
#  define MB_APB2ENR_BITS_2	0
#  define MB_USART_BUS_FREQ	APB1_FREQ
#endif

void MB_DRVINIT (mudbus_driver_t *mbd)
{
    mbd->usart = USART (MB);
    mbd->dma = DMA (MB_USART_TX);

    // Включим питание контроллеру DMA1
    RCC->AHBENR |= JOIN3 (RCC_AHBENR_DMA, DMA_NUM (MB_USART_TX), EN);

    // Enable clocking GPIO, USART and AFIO
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | MB_APB2ENR_BITS | MB_APB2ENR_BITS_2;
    RCC->APB1ENR |= MB_APB1ENR_BITS;

    // Set the output bit on TX to enable pull-up when we're in mute mode
    GPIO_BSET (MB_USART_TX);

    mbd->gpio_tx_cr = GPIO_CR (MB_USART_TX);
    mbd->gpio_tx_bit = BIT (MB_USART_TX);
    mbd->dma_tx_chan = DMA_CHAN (MB_USART_TX);
    mbd->dma_rx_chan = DMA_CHAN (MB_USART_RX);

    // Initialize RX and TX
    mbd_tx_mute (mbd, false);
    *GPIO_CR (MB_USART_RX) = GPIO_SET (
        *GPIO_CR (MB_USART_RX), MB_USART_RX, INPUT, FLOATING);

    // Initialize the USART
    usart_init (USART (MB), MB_USART_BUS_FREQ, JOIN3 (USART, MB_USART, _SETUP));
    // Generate IRQs on RX errors
    USART (MB)->CR3 |= USART_CR3_EIE;
    // And on IDLE line
    USART (MB)->CR1 |= USART_CR1_IDLEIE;

    // Set up the DMA TX and RX IRQ handlers
    nvic_setup (DMA_IRQ (MB_USART_TX), DMA_IRQ_PRIO (MB_USART_TX));
    nvic_setup (DMA_IRQ (MB_USART_RX), DMA_IRQ_PRIO (MB_USART_RX));
    // And the USART IRQ handler too
    nvic_setup (JOIN3 (USART, MB_USART, _IRQn), JOIN3 (USART, MB_USART, _IRQ_PRIO));
}

DMA_IRQ_HANDLER (MB_USART_TX)
{
    uint32_t isr = DMA (MB_USART_TX)->ISR;

    // Transfer error?
    if (isr & DMA_ISR (MB_USART_TX, TEIF))
    {
        // Acknowledge the interrupt
        DMA (MB_USART_TX)->IFCR = DMA_IFCR (MB_USART_TX, CTEIF);

        mb_send_stop (&MB_VAR);
    }

    // Transfer complete?
    if (isr & DMA_ISR (MB_USART_TX, GIF))
    {
        // Acknowledge the interrupt
        DMA (MB_USART_TX)->IFCR = DMA_IFCR (MB_USART_TX, CGIF);

        // Disable USART -> DMA transmission
        USART (MB)->CR3 &= ~USART_CR3_DMAT;

        mb_send_next (&MB_VAR);
    }
}

DMA_IRQ_HANDLER (MB_USART_RX)
{
    uint32_t isr = DMA (MB_USART_RX)->ISR;

    // Transfer error?
    if (isr & DMA_ISR (MB_USART_TX, TEIF))
    {
        // Acknowledge the interrupt
        DMA (MB_USART_TX)->IFCR = DMA_IFCR (MB_USART_TX, CTEIF);

        mb_recv_error (&MB_VAR);
    }

    if (isr & DMA_ISR (MB_USART_RX, GIF))
    {
        // Acknowledge the interrupt
        DMA (MB_USART_RX)->IFCR = DMA_IFCR (MB_USART_RX, CGIF);

        // Disable DMA -> USART transmission
        USART (MB)->CR3 &= ~USART_CR3_DMAR;

        // Send next frag
        mb_recv_next (&MB_VAR);
    }
}

void JOIN3 (USART, MB_USART, _IRQHandler) ()
{
    uint32_t sr = USART (MB)->SR;

    if ((sr & USART_SR_TC) && (USART (MB)->CR1 & USART_CR1_TCIE))
    {
        // Disable TC interrupt, will re-enable if required
        USART (MB)->CR1 &= ~USART_CR1_TCIE;
        // TX complete
        mb_send_next (&MB_VAR);
    }

    if (sr & (USART_SR_FE | USART_SR_NE | USART_SR_ORE | USART_SR_IDLE))
    {
        if (sr & USART_SR_IDLE)
            // Clear the IDLE bit with a void read from DR
            (void)USART (MB)->DR;

        // Receiver error or IDLE pseudo-symbol received, restart
        mb_recv_error (&MB_VAR);
    }
}

#undef MB_USART_BUS_FREQ
#undef MB_APB1ENR_BITS
#undef MB_APB2ENR_BITS
#undef MB_APB2ENR_BITS_2
#undef MB_USART_TX
#undef MB_USART_RX
#undef MB_DRVINIT
#undef MB_USART
#undef MB_VAR
