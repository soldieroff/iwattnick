/*
 * Эта программа фактически дублирует функциональность прошивки
 * по умолчанию для STM32VL-DISCOVERY (tvld).
 *
 * Правда, размер в два с лишним раза меньше :)
 */

#include HARDWARE_H

// Инкрементируется 8 раз в секунду
static volatile uint32_t clock;

int main ()
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // --- GPIOs (defined in hardware*.h) --- //

    // Включим тактирование GPIO для обоих светодиодов и кнопки
    RCC->APB2ENR |=
        JOIN3 (RCC_APB2ENR_IOP, PORT (GLED), EN) |
        JOIN3 (RCC_APB2ENR_IOP, PORT (USRBUT), EN);

#if !PORTS_CMP (GLED, BLED)
#error "Expected GLED and BLED on same port!"
#endif
    *GPIO_CR (GLED) = GPIO_SET (GPIO_SET (*GPIO_CR (GLED),
        GLED, OUTPUT_2MHz, PUSHPULL),
        BLED, OUTPUT_2MHz, PUSHPULL);

    // Выключим лампочки, мало ли что
    GPIO (GLED)->BRR = BITV (GLED) | BITV (BLED);

    // Настроим GPIO для кнопки - INPUT Z-state
    *GPIO_CR (USRBUT) = GPIO_SET (*GPIO_CR (USRBUT),
        USRBUT, INPUT, FLOATING);

    // Настроим системный таймер на 32 прерывания в секунду
    if (SysTick_Config (SystemCoreClock / 32))
    {
        /* Capture error */
        for (;;) ;
    }

    uint32_t counter = 0, speed = 0;
    uint32_t old_bst = 0;
    uint32_t old_clock = clock;
    for (;;)
    {
        // Если прошло 1/8 секунды, за работу!
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
                {
                    GPIO (BLED)->BSRR = BITV (BLED);
                    speed = (speed + 1) & 3;
                }
                else
                    GPIO (BLED)->BRR = BITV (BLED);
            }

            counter++;
            if (counter & ((32/8) << speed))
                GPIO (GLED)->BSRR = BITV (GLED);
            else
                GPIO (GLED)->BRR = BITV (GLED);
        }

        // До следующего прерывания нам делать абсолютно нечего
        __WFI ();
    }
}

void SysTick_Handler ()
{
    clock++;
}
