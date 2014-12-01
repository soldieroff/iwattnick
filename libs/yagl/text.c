/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"

uint32_t g_glyph (int x, int y, uint8_t glyph)
{
    const uint8_t *data = gbc_glyph (glyph);
    if (!data)
        return g_user_glyph (x, y, glyph);

    if (gbc_glyph_types [glyph >> 3] & (1 << (glyph & 7)))
    {
        unsigned nframes = (unsigned)data [0] + 1;
        unsigned delay = (unsigned)data [1] + 1;
        return g_anim (x, y, (clock / delay) % nframes, data);
    }

    return g_bitmap (x, y, data);
}

uint32_t g_print (int x, int y, uint8_t fspc, uint8_t count, const char *text)
{
    int spacing = (int8_t)((fspc & 127) | ((fspc & 64) << 1));
    unsigned mw = 0, mh = 0;
    while (count--)
    {
        uint32_t size = g_glyph (x, y, *text++);
        if (fspc & G_TEXTF_VERTICAL)
        {
            unsigned delta = G_SIZE_H (size) + spacing;
            y += delta;
            mh += delta;
            delta = G_SIZE_W (size);
            if (delta > mw)
                mw = delta;
        }
        else
        {
            unsigned delta = spacing + G_SIZE_W (size);
            x += delta;
            mw += delta;
            delta = G_SIZE_H (size);
            if (delta > mh)
                mh = delta;
        }
    }

    return G_MK_SIZE (mw, mh);
}

uint32_t g_text (int x, int y, const uint8_t *text)
{
    uint8_t fspc = text [0];
    unsigned count = text [1] + 1;
    return g_print (x, y, fspc, count, (const char *)(text + 2));
}
