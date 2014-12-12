/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"

void g_rect_init (g_rect_t *r, int x, int y)
{
    r->xmin = x;
    r->xmax = x - 1;
    r->ymin = y;
    r->ymax = y - 1;
}

void g_rect_extend (g_rect_t *r, int x, int y)
{
    if (r->xmin > x)
        r->xmin = x;
    if (r->xmax < x)
        r->xmax = x;
    if (r->ymin > y)
        r->ymin = y;
    if (r->ymax < y)
        r->ymax = y;
}

uint32_t g_rect_size (g_rect_t *r)
{
    return G_MK_SIZE (r->xmax - r->xmin + 1, r->ymax - r->ymin + 1);
}
