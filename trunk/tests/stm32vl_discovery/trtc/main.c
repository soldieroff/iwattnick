#include "stm32vl_discovery.h"
#include "rtc.h"
#include "datetime.h"

/// The base unixtime for RTC clock
uint32_t rtc_basetime = 1414489182;

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
            {
                GPIO (GLED)->BSRR = BITV (GLED);

                datetime_t dt;
                uint32_t ut = rtc_counter ();
                ut2dt (rtc_basetime + (ut >> 4), &dt);
                printf ("(%d) %04d/%02d/%02d %02d:%02d:%02d.%d\r",
                    ut,
                    dt.year, dt.mon + 1, dt.day,
                    dt.hour, dt.min, dt.sec,
                    ((ut & 15) * 10) / 16);

                GPIO (GLED)->BRR = BITV (GLED);
            }
        }

        // До следующего прерывания нам делать абсолютно нечего
        __WFI ();
    }
}
