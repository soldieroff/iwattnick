/*
 * Демонстрация некоторых возможностей библиотеки для упрощения
 * работы с последовательными портами ввода/вывода (USART)
 */

#include "stm32vl_discovery.h"

static volatile uint32_t clock;

void SysTick_Handler(void)
{
    clock++;
}

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // AFIO_MAPR_SWJ_CFG_DISABLE

    // Инициализируем USART1
    usart1_init ();
    printf ("USART library demo running\r\n");

    led_init ();

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config (SystemCoreClock / 16))
    {
        /* Capture error */
        for (;;) ;
    }

    uint32_t old_clock = clock;
    uint32_t old_bst = 0;
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
            {
                GPIO (GLED)->BSRR = BITV (GLED);
                printf ("[%d]", old_clock >> 4);
                GPIO (GLED)->BRR = BITV (GLED);
            }

            // Check the button
            uint32_t bst = GPIO (USRBUT)->IDR & BITV (USRBUT);
            if (bst != old_bst)
            {
                old_bst = bst;

                // If pressed, light the blue LED
                if (bst)
                    GPIO (BLED)->BSRR = BITV (BLED);
                else
                    GPIO (BLED)->BRR = BITV (BLED);

                printf ("[button %d]", bst);
            }
        }
    }
}
