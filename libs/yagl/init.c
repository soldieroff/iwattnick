/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "stdfun.h"

// The global graphics context
g_t g;

void g_init ()
{
    memset (&g, 0, sizeof (g));
    g.color = (1 << G_BPP) - 1;
    g_clip_screen ();

    JOIN2 (G_HARDWARE, _init) ();
}

void g_clear ()
{
    memset (g.fb, 0, sizeof (g.fb));
}
