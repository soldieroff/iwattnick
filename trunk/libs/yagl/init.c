/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "stdfun.h"

g_t g;

void g_init ()
{
    memset (&g, 0, sizeof (g));
    g.clip.xmax = G_FB_W - 1;
    g.clip.ymax = G_FB_H - 1;
    g.color = G_PM (0, 0);

    JOIN2 (G_HARDWARE, _init) ();
}

void g_clear ()
{
    memset (g.fb, 0, sizeof (g.fb));
}
