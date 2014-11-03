#include "stm32vl_discovery.h"
#include "nvic.h"
#include "dma.h"
#include "mudbus.h"
#include "gears.h"

DMA_IRQ_HANDLER (MBM_USART_TX)
{
    if (DMA (MBM_USART_TX)->ISR & DMA_ISR (MBM_USART_TX, GIF))
    {
        // Acknowledge the interrupt
        DMA (MBM_USART_TX)->IFCR = DMA_IFCR (MBM_USART_TX, CGIF);

        // Disable USART1 -> DMA transmission
        USART (MBM)->CR3 &= ~USART_CR3_DMAT;
    }
}

DMA_IRQ_HANDLER (MBM_USART_RX)
{
    if (DMA (MBM_USART_TX)->ISR & DMA_ISR (USART1_RX, GIF))
    {
        // Acknowledge the interrupt
        DMA (MBM_USART_TX)->IFCR = DMA_IFCR (USART1_RX, CGIF);

        // Disable USART1 -> DMA transmission
        USART (MBM)->CR3 &= ~USART_CR3_DMAR;
    }
}

uint32_t ost_sec;

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Инициализация стандартной периферии STM32VL-DISCOVERY
    clock_init ();
    led_init ();

    // Инициализация MudBus Master'а
    mbm_init ();

    // Настроим и включим прерывания
    nvic_setup (DMA_IRQ (MBM_USART_TX), DMA_IRQ_PRIO (MBM_USART_TX));
    nvic_setup (DMA_IRQ (MBM_USART_RX), DMA_IRQ_PRIO (MBM_USART_RX));
    __enable_irq ();

    ost_disable (&ost_sec);

    for (;;)
    {
        if (ost_expired (&ost_sec))
        {
            ost_arm (&ost_sec, CLOCKS (1.0));
            GPIO (BLED)->ODR ^= BITV (BLED);

            mbm_send ((uint8_t *)"head [", 6);
            mbm_send ((uint8_t *)"body", 4);
            mbm_send ((uint8_t *)"] tail", 6);
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
