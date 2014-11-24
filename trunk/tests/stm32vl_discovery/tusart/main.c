/*
 * Демонстрация некоторых возможностей библиотеки для упрощения
 * работы с последовательными портами ввода/вывода (USART)
 */

#include "stm32vl_discovery.h"

static void do_test_xsend ()
{
    for (;;)
    {
        if (usart_rx_ready (USART1))
            switch (usart_getc (USART1))
            {
                case 'q':
                    return;

                default:
                    puts ("\r\nq: Quit test");
                    break;
            }

        puts ("\r\nSending 1000 visible characters ...");
        for (unsigned i = 0; i < 1000; i++)
            putc ('.');

        puts ("\r\nNow sending 1000 visible characters with TX off...");

        // Переводим вывод USART1_TX в режим плавающего входа
        *GPIO_CR (USART1_TX) = GPIO_CONFIGURE (*GPIO_CR (USART1_TX),
            USART1_TX, INPUT, FLOATING);

        for (unsigned i = 0; i < 1000; i++)
            putc ('@');

        // Подождём пока данные уйдут из выходного FIFO и DR.
        while (!usart_tx_complete (USART1))
            ;

        // Возвращаем USART1_TX в режим AF push/pull
        *GPIO_CR (USART1_TX) = GPIO_CONFIGURE (*GPIO_CR (USART1_TX),
            USART1_TX, OUTPUT_2MHz, AF_PUSHPULL);
    }
}

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // AFIO_MAPR_SWJ_CFG_DISABLE

    // Инициализируем USART1
    usart1_init ();
    puts ("USART library demo running");

    clock_init ();
    led_init ();

    uint32_t old_clock = clock;
    uint32_t old_bst = 0;
    for (;;)
    {
        if (usart_rx_ready (USART1))
            switch (usart_getc (USART1))
            {
                case 'x':
                    do_test_xsend ();
                    break;

                default:
                    puts ("\r\nx: Test transmitter with output disabled");
                    break;
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
