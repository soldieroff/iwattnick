/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"

void g_rect (int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
        XCHG (x1, x2);
    if (y1 > y2)
        XCHG (y1, y2);

    g_hline (x1, x2, y1);
    g_hline (x1, x2, y2);
    g_vline (x1, y1, y2);
    g_vline (x2, y1, y2);
}

void g_box (int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
        XCHG (x1, x2);
    if (y1 > y2)
        XCHG (y1, y2);

#if G_BORD == G_BORD_VLSBT
    while (x1 <= x2)
    {
        g_vline (x1, y1, y2);
        x1++;
    }
#else
    while (y1 <= y2)
    {
        g_hline (x1, x2, y1);
        y1++;
    }
#endif
}
