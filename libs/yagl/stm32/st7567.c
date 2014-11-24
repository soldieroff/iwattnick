/*
    ST7567 LCD controller library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "gears.h"
#include "stdfun.h"
#include "printf.h"

// the next page to upload (0 ... G_FB_H/8-1)
static struct
{
    uint8_t page;
    uint8_t setcol1;
    uint8_t setcol2;
} PACKED txfb;

// ST7567 controller initialization command sequence
static const uint8_t st7567_init_seq [] =
{
    // enable power VB,VR,VF
    ST7567_PWR_CTRL (1, 1, 1),
    // Coarse contrast, range 0x20 ~ 0x27, low to high
    ST7567_CONTRAST_R (3),
    // Fine contrast, range 0x00 ~ 0x3f, low to high
    ST7567_CONTRAST_F (0x18),
    // 1/9 ratio bias (bias)
    ST7567_SET_BIAS (0),
    // line scan order: top to bottom
    ST7567_VERT_DIR (1),
    // column scanning order: left to right
    ST7567_HORIZ_DIR (0),
    // start line: the first line
    ST7567_START_LINE (0),
    // enable display
    ST7567_DISP_ENABLE (1),
};

static const uint16_t st7567_gpio_config [] =
{
    GPIO_CONFIG (ST7567_CS, OUTPUT_50MHz, PUSHPULL, 1),
    GPIO_CONFIG (ST7567_RES, OUTPUT_50MHz, PUSHPULL, 0),
    GPIO_CONFIG (ST7567_RS, OUTPUT_50MHz, PUSHPULL, X),
    GPIO_CONFIG (ST7567_SDA, OUTPUT_50MHz, AF_PUSHPULL, X),
    GPIO_CONFIG (ST7567_SCK, OUTPUT_50MHz, AF_PUSHPULL, X),
};

#if SPI_NUM (ST7567) == 1
#  define __ST7567_APB1ENR	0
#  define __ST7567_APB2ENR	RCC_APB2ENR_SPI1EN
#elif SPI_NUM (ST7567) == 2 || SPI_NUM (ST7567) == 3
#  define __ST7567_APB1ENR	JOIN3 (RCC_APB1ENR_SPI, SPI_NUM (ST7567), EN)
#  define __ST7567_APB2ENR	0
#endif

void st7567_init ()
{
    // Включим тактирование альтернативных функций и SPI
#if __ST7567_APB1ENR != 0
    RCC->APB1ENR |= __ST7567_APB1ENR;
#endif
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | __ST7567_APB2ENR;
    // Включим тактирование контроллера DMA
    RCC->AHBENR |= JOIN3 (RCC_AHBENR_DMA, DMA_NUM (ST7567_SPI_TX), EN);

    // Настроим все GPIO, к которым подключён экран
    gpio_configs (st7567_gpio_config, ARRAY_LEN (st7567_gpio_config));

    //--- Инициализируем SPI ---//
    SPI (ST7567)->CR1 = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE |
        SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CLK_4 | SPI_CR1_CPOL | SPI_CR1_CPHA;
    SPI (ST7567)->CR2 = 0;

    // Включаем SPI в режиме master
    SPI (ST7567)->CR1 |= SPI_CR1_SPE | SPI_CR1_MSTR;

    // Настроить IRQ для обработки сигнала от DMA TX
    nvic_setup (DMA_IRQ (ST7567_SPI_TX), DMA_IRQ_PRIO (ST7567_SPI_TX));

    // Снимем с ЖК сигнал сброса, RES -> 1
    GPIO_SET (ST7567_RES);

    // Инициализация контроллера последовательностью команд
    st7567_send (true, st7567_init_seq, ARRAY_LEN (st7567_init_seq));
}

bool st7567_send (bool command, const uint8_t *data, unsigned len)
{
    if (!st7567_idle ())
        return false;

    /* К сожалению, STM32 не имеет возможности вызывать прерывание по флагу BSY.
     * Поэтому мы будем просто передёргивать (Slave|Chip) Select каждый раз,
     * когда посылаем новую команду. Таким образом мы подстрахуемся от
     * возможной десинхронизации сдвигового регистра в ST7567 из-за помех.
     */
    GPIO_SET (ST7567_CS);

    if (command)
        GPIO_RESET (ST7567_RS);
    else
        GPIO_SET (ST7567_RS);

    SPI (ST7567)->CR2 |= SPI_CR2_TXDMAEN;

    GPIO_RESET (ST7567_CS);

    dma_copy (DMA (ST7567_SPI_TX), DMA_CHAN (ST7567_SPI_TX),
        DMA_CCR_TCIE | DMA_CCR_MINC | DMA_CCR_PSIZE_8 | DMA_CCR_PL_LOW,
        (void *)data, &SPI (ST7567)->DR, len);

    return true;
}

DMA_IRQ_HANDLER (ST7567_SPI_TX)
{
    uint32_t isr = DMA (ST7567_SPI_TX)->ISR;

    // Transfer error?
    if (isr & DMA_ISR (ST7567_SPI_TX, TEIF))
    {
        // Acknowledge the interrupt
        DMA (ST7567_SPI_TX)->IFCR = DMA_IFCR (ST7567_SPI_TX, CTEIF);

        SPI (ST7567)->CR2 &= ~SPI_CR2_TXDMAEN;
    }

    if (isr & DMA_ISR (ST7567_SPI_TX, GIF))
    {
        // Acknowledge the interrupt
        DMA (ST7567_SPI_TX)->IFCR = DMA_IFCR (ST7567_SPI_TX, CGIF);

        SPI (ST7567)->CR2 &= ~SPI_CR2_TXDMAEN;

        if (txfb.page)
        {
            // If we just sent the SET_PAGE command, now send the data
            if (txfb.page & ~15)
            {
                txfb.page &= 15;
                st7567_send (false, g.fb + txfb.page * G_FB_W, G_FB_W);
                txfb.page++;
                if (txfb.page >= (G_FB_H + 7) / 8)
                    txfb.page = 0;
            }
            else
            {
                txfb.page = ST7567_SET_PAGE (txfb.page);
                st7567_send (true, &txfb.page, 3);
            }
        }
    }
}

bool st7567_idle ()
{
    if (SPI (ST7567)->CR2 & SPI_CR2_TXDMAEN)
        return false;

    if (SPI (ST7567)->SR & SPI_SR_BSY)
        return false;

    return true;
}

void st7567_refresh ()
{
    txfb.page = ST7567_SET_PAGE (0);
    txfb.setcol1 = ST7567_SET_COLUMN (1, 0);
    txfb.setcol2 = ST7567_SET_COLUMN (0, 0);
    st7567_send (true, &txfb.page, 3);
}
