#include HARDWARE_H
#include "serio.h"
#include "printf.h"

static uint32_t clock;

void SysTick_Handler(void)
{
    clock++;
}

void usart1_init ()
{
    // Включаем тактирование GPIOA, USART1 и альтернативных функций AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;

#if !PORTS_CMP (USART1_TX, USART1_RX)
#error "Expected USART1 RX & TX on same port!"
#endif
    // Инициализация вывода PA9 & PA10 - USART1_Tx & USART1_Rx
    *GPIO_CR (USART1_TX) = GPIO_SET (GPIO_SET (*GPIO_CR (USART1_TX),
        USART1_TX, OUTPUT_2MHz, AF_PUSHPULL),
        USART1_RX, INPUT, FLOATING);

    // USART1 is on APB2
    serio_init (USART1, APB2_FREQ,
        9600 | USART_CHARBITS_8 | USART_PARITY_NONE | USART_STOPBITS_1);

    stdio_init (USART1);
}

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // AFIO_MAPR_SWJ_CFG_DISABLE

    // Инициализируем USART1
    usart1_init ();

    printf ("USART library demo running\r\n");

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config (SystemCoreClock / 16))
    {
        /* Capture error */
        for (;;) ;
    }

    uint32_t old_clock = clock;
    for (;;)
    {
        if (serio_iready (USART1))
        {
            char c = serio_getc (USART1);
            printf ("[got '%c']", c);
        }

        if (old_clock != clock)
        {
            old_clock = clock;
            if (!(old_clock & 15))
                printf ("[%d]", old_clock >> 4);
        }
    }
}
