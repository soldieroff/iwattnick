#include "stm32vl_discovery.h"
#include "rtc.h"

static volatile uint32_t clock;

void SysTick_Handler(void)
{
    clock++;
}

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    usart1_init ();
    printf ("Real-Time-Clock demo started\r\n");

    led_init ();
    rtc_init ();

    /* Setup SysTick Timer for 16 interrupts/sec */
    if (SysTick_Config (SystemCoreClock / 16))
    {
        /* Capture error */
        for (;;) ;
    }

    uint32_t old_bst = 0;
    uint32_t old_clock = clock;
    for (;;)
    {
        // flash the green LED!
        if (old_clock != clock)
        {
            old_clock = clock;

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
            }

            if ((old_clock & 15) == 0)
                printf ("%d\r", rtc_counter ());
        }

        // До следующего прерывания нам делать абсолютно нечего
        __WFI ();
    }
}
