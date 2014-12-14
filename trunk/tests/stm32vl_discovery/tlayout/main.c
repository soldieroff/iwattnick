#include "stm32vl_discovery.h"
#include "gears.h"
#include "usefun.h"
#include "yagl.h"
#include "glayout.h"

#include "gfx.h"

// declare the menus[] array which contains the state of every menu
IMPLEMENT_MENUS;
// declare the layouts[] array which contains the state of every layout
IMPLEMENT_LAYOUTS;

#define FPS 16

// one-shot-timer that shoots to mark every frame
uint32_t ost_frame;
// test variable
int v_test;

int i2s (int n, char *buff, unsigned buffsize)
{
    unsigned x = 0;
    if (n < 0)
    {
        buff [0] = 10;
        n = -n;
        x = 1;
    }

    unsigned c = buffsize;
    while (c > 1)
    {
        buff [--c] = (n % 10);
        n /= 10;
        if (n == 0)
            break;
    }

    memcpy (buff + x, buff + c, buffsize - c);
    return x + buffsize - c;
}

uint32_t g_user_glyph (int x, int y, uint32_t glyph)
{
    char buff [8];
    int n;

    switch (glyph & 0xff)
    {
        case VAR_speed:
            n = i2s ((g_clock & 255) - 128, buff, sizeof (buff));
            return g_printa (x, y, 1, n, buff, glyph >> 8);

        case VAR_dist:
            n = i2s ((g_clock >> 4) & 127, buff, sizeof (buff));
            return g_printa (x, y, 1, n, buff, glyph >> 8);

        case VAR_test:
            n = i2s (v_test, buff, sizeof (buff));
            return g_printa (x, y, 1, n, buff, glyph >> 8);

        default:
            return 0;
    }
}

uint8_t active_layout;

void g_user_action (unsigned action, int *args)
{
    switch (action)
    {
        case ACTION_layout:
            g_layouts [args [0]].prev = active_layout;
            active_layout = args [0];
            break;

        case ACTION_setvar:
            if (args [0] == VAR_test)
                v_test = args [1];
            break;

        case ACTION_back:
            active_layout = g_layouts [active_layout].prev;
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
    printf ("YAGL layouts test started\r\n");

    // Настроим и включим прерывания
    __enable_irq ();

    // Инициализация графической библиотеки
    g_init ();

    ost_disable (&ost_frame);

    active_layout = LAYOUT_test1;

    for (;;)
    {
        while (usart_rx_ready (USART1))
        {
            uint8_t menu = g_layouts [active_layout].menu;
            switch (usart_getc (USART1))
            {
                case 'z':
                    g_menu_event (menu, G_MENU_UP);
                    break;

                case 'x':
                    g_menu_event (menu, G_MENU_DOWN);
                    break;

                case 'q':
                    g_menu_event (menu, G_MENU_FIRST);
                    break;

                case 'w':
                    g_menu_event (menu, G_MENU_LAST);
                    break;

                case 'a':
                    g_menu_event (menu, G_MENU_ACTIVATE);
                    break;

                case 's':
                    g_user_action (ACTION_back, 0);
                    break;
            }
        }

        if (ost_expired (&ost_frame))
        {
            // По нажатию кнопки замораживаем картинку
            if (GPIO (USRBUT)->IDR & BITV (USRBUT))
            {
                while (GPIO (USRBUT)->IDR & BITV (USRBUT))
                    ;
            }

            g_clock = clock;
            ost_arm (&ost_frame, CLOCKS (1.0/FPS));
            GPIO (BLED)->ODR ^= BITV (BLED);

            g_clear ();
            g_layout_draw (0, 0, active_layout);
            g_refresh ();
        }

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
