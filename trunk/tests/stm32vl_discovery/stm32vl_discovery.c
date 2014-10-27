/*
 * STM32VL-DISCOVERY specific functions.
 * Copyright (C) 2014 Andrey Zabolotnyi
 */

#include "stm32vl_discovery.h"

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

    // Включаем printf() через USART1
    stdio_init (USART1);
}

void led_init ()
{
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

    // Настроим GPIO для кнопки
    *GPIO_CR (USRBUT) = GPIO_SET (*GPIO_CR (USRBUT),
        USRBUT, INPUT, FLOATING);
}
