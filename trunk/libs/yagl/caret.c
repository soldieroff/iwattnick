/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "glayout.h"
#include "yaglp.h"

void g_caret_init (g_caret_t *c, int x, int y, int8_t spacing)
{
    c->x = x;
    c->y = y;
    c->spacing = spacing;
    c->flags = 0;
    g_rect_init (&c->b, x, y);
}

void g_caret_advance (g_caret_t *c, uint32_t wh)
{
    unsigned w = G_SIZE_W (wh);
    unsigned h = G_SIZE_H (wh);

    switch (c->flags & G_FLAGS_DIRMASK)
    {
        case G_DIR_RIGHT:
            c->x += w;
            g_rect_extend (&c->b, c->x - 1, c->y + h - 1);
            c->x += c->spacing;
            break;

        case G_DIR_DOWN:
            c->y += h;
            g_rect_extend (&c->b, c->x + w - 1, c->y - 1);
            c->y += c->spacing;
            break;
    }
}
