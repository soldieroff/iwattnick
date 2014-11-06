#include "stm32vl_discovery.h"
#include "nvic.h"
#include "dma.h"
#include "mudbus.h"
#include "gears.h"

mudbus_t mb;

extern void mb_send_next (mudbus_t *mb);
extern void mb_send_stop (mudbus_t *mb);

DMA_IRQ_HANDLER (MBM_USART_TX)
{
    uint32_t isr = DMA (MBM_USART_TX)->ISR;

    // Transfer complete?
    if (isr & DMA_ISR (MBM_USART_TX, GIF))
    {
        // Acknowledge the interrupt
        DMA (MBM_USART_TX)->IFCR = DMA_IFCR (MBM_USART_TX, CGIF);

        // Disable USART1 -> DMA transmission
        USART (MBM)->CR3 &= ~USART_CR3_DMAT;

        mb_send_next (&mb);
    }
    // Transfer error?
    else if (isr & DMA_ISR (MBM_USART_TX, TEIF))
    {
        // Acknowledge the interrupt
        DMA (MBM_USART_TX)->IFCR = DMA_IFCR (MBM_USART_TX, CTEIF);

        // Disable USART1 -> DMA transmission
        USART (MBM)->CR3 &= ~USART_CR3_DMAT;

        mb_send_stop (&mb);
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
    mbm_init (&mb);

    // Настроим и включим прерывания
    nvic_setup (DMA_IRQ (MBM_USART_TX), DMA_IRQ_PRIO (MBM_USART_TX));
    nvic_setup (DMA_IRQ (MBM_USART_RX), DMA_IRQ_PRIO (MBM_USART_RX));
    __enable_irq ();

    ost_disable (&ost_sec);

    stdio_init (USART1);

    for (;;)
    {
        if (ost_expired (&ost_sec))
        {
            ost_arm (&ost_sec, CLOCKS (1.0));
            GPIO (BLED)->ODR ^= BITV (BLED);

            mb_send (&mb, (uint8_t *)"head [", 6);
            mb_send (&mb, (uint8_t *)"body", 4);
            mb_send (&mb, (uint8_t *)"] tail", 6);
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
