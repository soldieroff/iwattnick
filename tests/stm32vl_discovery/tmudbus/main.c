#include "stm32vl_discovery.h"
#include "mudbus.h"
#include "gears.h"

mudbus_t mb;

#define MB_DRVINIT	mbd_master_init
#define MB_USART	MBM_USART
#define MB_VAR		mb
#include "mudbus-stm32-gen.h"

uint32_t ost_sec;

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Инициализация стандартной периферии STM32VL-DISCOVERY
    clock_init ();
    led_init ();

    // Инициализация MudBus Master'а
    mbd_master_init (&mb.driver);
    mb_init (&mb);

    // Настроим и включим прерывания
    __enable_irq ();

    ost_disable (&ost_sec);

    stdio_init (USART1);

    for (;;)
    {
        if (ost_expired (&ost_sec))
        {
            ost_arm (&ost_sec, CLOCKS (1.0));
            GPIO (BLED)->ODR ^= BITV (BLED);

            mb_send (&mb, (uint8_t *)"head [", 6);
            mb_send (&mb, (uint8_t *)"body", 4);
            mb_send (&mb, (uint8_t *)"] tail", 6);
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
