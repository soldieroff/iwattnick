#include "stm32vl_discovery.h"
#include "dma.h"
#include "nvic.h"

static const char test_data [] = "[This data came via DMA controller]\r\n";

DMA_IRQ_HANDLER (USART1_TX)
{
    if (DMA (USART1_TX)->ISR & DMA_ISR (USART1_TX, GIF))
    {
        // Acknowledge the interrupt
        DMA (USART1_TX)->IFCR = DMA_IFCR (USART1_TX, CGIF);

        // Disable USART1 -> DMA transmission
        USART1->CR3 &= ~USART_CR3_DMAT;

        printf ("</dma>\r\n");
    }
}

static void do_test_send ()
{
    printf ("Sending some data via DMA ... NOW!\r\n<dma>\r\n");

    // Clear "Transmission Complete" USART flag
    USART1->SR &= ~USART_SR_TC;
    // Enable USART1 -> DMA transmission
    USART1->CR3 |= USART_CR3_DMAT;

    // rather than writing directly "dma1_copy" we'll use our brain-fucking macros
    JOIN3 (dma, DMA_NUM (USART1_TX), _copy) (DMA_CHAN (USART1_TX),
        DMA_CCR_PSIZE_8 | DMA_CCR_MSIZE_8 | DMA_CCR_PL_VERYHIGH | DMA_CCR_TCIE,
        test_data, &USART1->DR, sizeof (test_data) - 1);

    // Wait until the data finishes transfer
    while ((DMA (USART1_TX)->ISR & DMA_ISR (USART1_TX, TCIF)) == 0)
        ;

    printf ("Got something?\r\n");
}

static void do_test_recv ()
{
}

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Включим питание контроллеру DMA1
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    usart1_init ();
    printf ("DMA demo started\r\n");

    led_init ();

    // Настроим и включим прерывания
    nvic_setup (DMA_IRQ (USART1_TX), IRQ_PRIO (USART1_TX_DMA));
    __enable_irq ();

    for (;;)
    {
        if (serio_iready (USART1))
            switch (serio_getc (USART1))
            {
                case 's':
                    do_test_send ();
                    break;

                case 'r':
                    do_test_recv ();
                    break;

                default:
                    printf ("s: Send test data via USART1+DMA\r\n"
                            "r: Receive data via USART1+DMA\r\n");
                    break;
            }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
