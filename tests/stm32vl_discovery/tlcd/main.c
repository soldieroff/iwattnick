#include "stm32vl_discovery.h"
#include "gears.h"
#include "stdfun.h"
#include "yagl.h"

static uint8_t catrun [6][20] =
{
    { 32, 160, 86, 118, 63, 60, 28, 28, 28, 28, 30, 14, 14, 12, 30, 26, 26, 25, 25, 1 },
    { 0, 0, 12, 14, 14, 30, 252, 252, 62, 46, 15, 15, 15, 63, 11, 18, 2, 2, 1, 0 },
    { 0, 0, 12, 12, 14, 28, 28, 60, 124, 126, 126, 62, 12, 12, 4, 4, 4, 6, 2, 0 },
    { 0, 0, 6, 7, 7, 62, 126, 28, 60, 188, 252, 248, 48, 32, 48, 16, 24, 16, 0, 0 },
    { 0, 0, 38, 55, 31, 127, 62, 28, 28, 60, 184, 248, 56, 120, 208, 144, 16, 16, 16, 0 },
    { 32, 48, 18, 119, 63, 31, 30, 28, 28, 28, 28, 28, 60, 252, 124, 36, 108, 76, 8, 8 },
};

uint32_t ost_frame;
uint8_t dmode;
uint8_t x, y, cp, cx;

static void switch_mode (uint8_t mode)
{
    if (mode > 1)
        mode = 0;

    switch (mode)
    {
        case 0:
            memset (g.fb, 0xff, G_FB_SIZE);
            x = y = cp = cx = 0;
            break;

        case 1:
            y = ST7567_START_LINE (0);
            st7567_send (true, &y, 1);
            break;
    }

    dmode = mode;
}

static void display ()
{
    switch (dmode)
    {
        case 0:
            for (uint8_t b = 0; b < 8; b++)
                g.fb [b * G_FB_W + x] = rand ();
            x = (x + 1) & 127;
            for (uint8_t b = 0; b < 8; b++)
                g.fb [b * G_FB_W + x] = 255;

            for (uint8_t i = 0; i < 20; i++)
                g.fb [4 * G_FB_W + ((cx + i + 2) & 127)] = catrun [cp][19 - i];
            cx++; cp++; if (cp > 5) cp = 0;

            y = ST7567_START_LINE (y + 1);
            st7567_send (true, &y, 1);
            break;

        case 1:
            g_clear ();
            g_hline (0, 100, 1);
            g_vline (100, 3, 60);
            g_hline (0, 100, 62);
            g_pixel (0, 0);
            g_pixel (100, 0);
            g_pixel (99, 2);
            g_pixel (99, 61);
            g_pixel (0, 63);
            g_pixel (100, 63);
            g_line (3, 3, 97, 60);
            g_line (97, 3, 3, 60);
            g_box (2, 20, 22, 23);
            g_box (2, 30, 22, 53);
            break;
    }
}

int main (void)
{
    // Отключаем JTAG, оставляем только SWD
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Инициализация стандартной периферии STM32VL-DISCOVERY
    clock_init ();
    led_init ();

    usart1_init ();
    printf ("LCD test started\r\n");

    // Настроим и включим прерывания
    __enable_irq ();

    // Инициализация графической библиотеки
    g_init ();

    ost_disable (&ost_frame);

    switch_mode (0);

    for (;;)
    {
        // По нажатию кнопки замораживаем картинку
        if (GPIO (USRBUT)->IDR & BITV (USRBUT))
        {
            while (GPIO (USRBUT)->IDR & BITV (USRBUT))
                ;
            switch_mode (dmode + 1);
        }

        if (ost_expired (&ost_frame))
        {
            ost_arm (&ost_frame, CLOCKS (1.0/16.0));
            GPIO (BLED)->ODR ^= BITV (BLED);

            display ();

            g_refresh ();
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
