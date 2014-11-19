/*
    STM32 GPIO helpers library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gpio.h"

void gpio_config (uint16_t conf)
{
    // Port number (0-A, 1-B ...)
    unsigned p = (conf & __GPIO_PORT_MASK) >> __GPIO_PORT_SHIFT;

#if defined __STM32F0XX_H || defined __STM32F30x_H
    RCC->AHBENR |= (RCC_AHBENR_GPIOAEN << p);
#elif defined __STM32F10x_H
    // Enable port clock
    RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN << p);
#elif defined __STM32F2xx_H || defined __STM32F4xx_H
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN << p);
#else
# error "No support yet to enable GPIO clock for current MCU"
#endif

    // GPIO bit number (0-15)
    unsigned b = (conf & __GPIO_BIT_MASK) >> __GPIO_BIT_SHIFT;
    uint32_t n = (conf & (GPIO_MODE_MASK | GPIO_CNF_MASK)) << ((b & 7) * 4);
    uint32_t m = ~((GPIO_MODE_MASK | GPIO_CNF_MASK) << ((b & 7) * 4));

    GPIO_TypeDef *gpio = (GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE - GPIOA_BASE) * p);
    if (b < 8)
        gpio->CRL = (gpio->CRL & m) | n;
    else
        gpio->CRH = (gpio->CRH & m) | n;
}

void gpio_configs (const uint16_t *conf, unsigned n)
{
    while (n--)
        gpio_config (*conf++);
}
