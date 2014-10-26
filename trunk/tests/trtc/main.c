#include HARDWARE_H

static volatile uint32_t clock;

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

#if 0
    // --- Real-Time Clock (RTC) --- //

    // Включим питание RTC
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    // Разрешим доступ на запись в регистры RTC
    PWR->CR |= PWR_CR_DBP;
    // Если часы не в нужном режиме работы, делаем полный сброс и конфигурируем заново
    if ((RCC->BDCR & (RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL)) !=
        (RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE))
    {
        // Включим внешний кварц и полностью сбросим часы и энергонезависимое хранилище
        RCC->BDCR = RCC_BDCR_LSEON | RCC_BDCR_BDRST;
        // Ждём пока кварц заведётся (можно не ждать, но для порядка...)
        while (!(RCC->BDCR & RCC_BDCR_LSERDY)) ;
        // Запустим тактирование RTC от внешнего кварца
        RCC->BDCR = RCC_BDCR_LSEON | RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;
    }
#endif

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
        USRBUT, INPUT, Z);

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config (SystemCoreClock / 8))
    {
        /* Capture error */
        for (;;) ;
    }

    uint32_t counter = 0, speed = 0;
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
                {
                    GPIO (BLED)->BSRR = BITV (BLED);
                    speed = (speed + 1) & 3;
                }
                else
                    GPIO (BLED)->BRR = BITV (BLED);
            }

            counter++;
            if (counter & (1 << speed))
                GPIO (GLED)->BSRR = BITV (GLED);
            else
                GPIO (GLED)->BRR = BITV (GLED);
        }
    }
}

void SysTick_Handler(void)
{
    clock++;
}
