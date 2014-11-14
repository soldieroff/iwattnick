#include "stm32vl_discovery.h"
#include "mudbus.h"
#include "gears.h"

#include <stddef.h>

mudbus_t mb;

#define MB_DRVINIT	mbd_init
#define MB_USART	MBM_USART
#define MB_VAR		mb
#include "drvgen.h"

/// Device identifier
const mb_devid_t mb_devid =
{
    /// firmware version, 4.4 hi/lo
    MB_VERSION (0, 1),
    /// Manufacturer ID
    MBMID_ZAP,
    /// Product ID
    MBPID_ZAP_IWATTNICK,
    /// Type ID
    MBTID_OTHER,
};

static const mb_cas_area_t mb_cas [] =
{
    { MB_CASA_DEVID | MBCAF_WP, MB_CASA_DEVID + sizeof (mb_devid), &mb_devid },
};

void mb_user_recv (mudbus_t *mb)
{
    switch (mb->inb [2] & MB_CMD_MASK)
    {
        case MBC_ERROR:
            break;

        case MBC_READ:
            mb_cmd_read (mb, mb_cas, ARRAY_LEN (mb_cas));
            break;

        case MBC_WRITE:
            mb_cmd_write (mb, mb_cas, ARRAY_LEN (mb_cas));
            break;

        case MBC_DATA:
            break;

        case MBC_EXEC:
            break;
    }
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

    stdio_init (USART1);

    // Инициализация MudBus Master'а
    mbd_init (&mb.driver);
    mb_init (&mb, '@');

    // Настроим и включим прерывания
    __enable_irq ();

    ost_disable (&ost_sec);

    for (;;)
    {
        if (ost_expired (&ost_sec))
        {
            ost_arm (&ost_sec, CLOCKS (1.0));
            GPIO (BLED)->ODR ^= BITV (BLED);

            mb_send_hdr (&mb, '@', MBC_READ, 2);
            mb_send_nofs (&mb, sizeof (mb_devid_t), MB_CASA_DEVID);
            mb_send_last (&mb, NULL, 0);
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
