/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "stdfun.h"

void g_clip_screen ()
{
    g.clip.xmin = g.clip.ymin = 0;
    g.clip.xmax = G_FB_W - 1;
    g.clip.ymax = G_FB_H - 1;
}

void g_clip (uint16_t cxmin, uint16_t cymin, uint16_t cxmax, uint16_t cymax)
{
    g.clip.xmin = cxmin;
    g.clip.ymin = cymin;
    g.clip.xmax = cxmax;
    g.clip.ymax = cymax;
}
