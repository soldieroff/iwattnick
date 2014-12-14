/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "usefun.h"

void g_clip_screen ()
{
    g.clip.xmin = g.clip.ymin = 0;
    g.clip.xmax = G_FB_W - 1;
    g.clip.ymax = G_FB_H - 1;
}

void g_clip_set (uint16_t xmin, uint16_t ymin, uint16_t xmax, uint16_t ymax)
{
    g.clip.xmin = xmin;
    g.clip.ymin = ymin;
    g.clip.xmax = xmax;
    g.clip.ymax = ymax;
    g_clip_limit ();
}

void g_clip_set_wh (uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    g.clip.xmin = x;
    g.clip.xmax = x + w - 1;
    g.clip.ymin = y;
    g.clip.ymax = y + h - 1;
    g_clip_limit ();
}

void g_clip_limit ()
{
    if (g.clip.xmin < 0)
        g.clip.xmin = 0;
    if (g.clip.ymin < 0)
        g.clip.ymin = 0;
    if (g.clip.xmax >= G_FB_W)
        g.clip.xmax = G_FB_W - 1;
    if (g.clip.ymax >= G_FB_H)
        g.clip.ymax = G_FB_H - 1;
}
