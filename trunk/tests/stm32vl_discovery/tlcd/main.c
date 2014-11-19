#include "stm32vl_discovery.h"
#include "gears.h"

#include <stddef.h>

static const uint16_t st7567_gpio_config [] =
{
    GPIO_CONFIG (ST7567_CS, OUTPUT_2MHz, PUSHPULL, 0),
    GPIO_CONFIG (ST7567_RES, OUTPUT_2MHz, PUSHPULL, 0),
    GPIO_CONFIG (ST7567_RS, OUTPUT_2MHz, PUSHPULL, 0),
    GPIO_CONFIG (ST7567_SDA, OUTPUT_2MHz, AF_PUSHPULL, X),
    GPIO_CONFIG (ST7567_SCK, OUTPUT_2MHz, AF_PUSHPULL, X),
};

void st7567_init ()
{
    // Включим тактирование альтернативных функций
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // Настроим все GPIO, к которым подключён экран
    gpio_configs (st7567_gpio_config, ARRAY_LEN (st7567_gpio_config));

    // Включим ЖК экран, RES -> 1
    GPIO_BSET (ST7567_RES);
}

uint32_t ost_sec;

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Инициализация стандартной периферии STM32VL-DISCOVERY
    clock_init ();
    led_init ();

    st7567_init ();

    usart1_init ();
    printf ("LCD test started\r\n");

    // Настроим и включим прерывания
    __enable_irq ();

    ost_disable (&ost_sec);

    for (;;)
    {
        if (ost_expired (&ost_sec))
        {
            ost_arm (&ost_sec, CLOCKS (1.0));
            GPIO (BLED)->ODR ^= BITV (BLED);

            putc ('.');
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
