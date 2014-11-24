/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "yaglp.h"

void g_pixel (int x, int y)
{
    // Trivial clipping tests
    if ((x < g.clip.xmin) ||
        (x > g.clip.xmax) ||
        (y < g.clip.ymin) ||
        (y > g.clip.ymax))
        return;

    _pixel (x, y, g.color);
}
