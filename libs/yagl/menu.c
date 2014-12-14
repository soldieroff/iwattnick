/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "glayout.h"
#include "usefun.h"

void g_menu_draw (int x1, int y1, int x2, int y2, uint8_t menu)
{
    unsigned menu_h = y2 - y1;

    // clip everything to menu area
    g_rect_t old_clip = g.clip;
    g_clip_set (x1, y1, x2, y2);

    const uint8_t *data = goc_menu (menu);

    unsigned nitems = *data++;
    int ih = uleb128 (&data);
    int ys = uleb128 (&data);
    g_menu_t *self = g_menus + menu;
    int sel_top_y = self->item * ih - self->yshift;
    int sel_bot_y = sel_top_y + ih - 1;

    int delta;
    if (sel_top_y < ih)
        delta = sel_top_y - ih;
    else if ((delta = (sel_bot_y - (menu_h - ih))) < 0)
    {
        delta = 0;
        self->yspeed = 0;
    }

    if (self->yspeed)
        // multiply by 4 for a faster menu, 2 for a slower but smoother
        delta = (delta * 4) / abs (self->yspeed);

    delta = self->yspeed + sign (delta - self->yspeed) +
        (delta - self->yspeed) / (G_FB_H / 8);
    if (delta > (G_FB_H / 4))
        delta = (G_FB_H / 4);
    if (delta < -(G_FB_H / 4))
        delta = -(G_FB_H / 4);
    self->yspeed = delta;
    self->yshift += self->yspeed;

    int y = y1 - self->yshift;
    for (unsigned i = 0; i < nitems; i++)
    {
        if (y > y2)
            break;

        uint16_t tofs = uleb128 (&data);
        int ny = y + ih;
        if (ny > y1)
        {
            g_color (1);
            if (i == self->item)
            {
                g_box (x1, y, x2, ny - 1);
                g_color (0);
            }
            g_text (x1, y + ys, goc_text (tofs));
        }

        y = ny;
    }

    // Restore clipping rectangle
    g.clip = old_clip;
}

void g_menu_event (uint8_t menu, g_menu_action_t action)
{
    g_menu_t *self = g_menus + menu;
    const uint8_t *data = goc_menu (menu);
    unsigned nitems = *data++;

    switch (action)
    {
        case G_MENU_UP:
            if (self->item)
            {
                self->item--;
                break;
            }
            // fallback
        case G_MENU_LAST:
            self->item = nitems - 1;
            break;

        case G_MENU_DOWN:
            if (self->item < nitems - 1)
            {
                self->item++;
                break;
            }
            // fallback
        case G_MENU_FIRST:
            self->item = 0;
            break;

        case G_MENU_ACTIVATE:
        {
            unsigned i, j;
            for (i = 2 + nitems; i > 0; i--)
                data = skip_leb128 (data);
            for (i = 0; i != self->item; i++)
                for (j = *data++ & 3; j > 0; j--)
                    data = skip_leb128 (data);

            int args [3];
            unsigned action = *data++;
            for (i = 0; i < (action & 3); i++)
                args [i] = sleb128 (&data);

            g_user_action (action, args);
            break;
        }
    }
}
