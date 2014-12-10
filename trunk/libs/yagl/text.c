/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "glayout.h"

uint32_t g_glyph (int x, int y, uint8_t glyph)
{
    const uint8_t *data = goc_glyph (glyph);
    if (goc_glyph_types [glyph >> 3] & (1 << (glyph & 7)))
    {
        unsigned nframes = (unsigned)data [1] + 1;
        unsigned delay = (unsigned)data [2] + 1;
        return g_anim (x, y, (clock / delay) % nframes, data);
    }

    return g_bitmap (x, y, data);
}

uint32_t g_print (int x, int y, int spacing, unsigned count, const char *text)
{
    unsigned mw = 0, mh = 0;
    while (count--)
    {
        uint8_t glyph = *text++;
        uint32_t size;
        if (glyph)
            size = g_glyph (x, y, glyph);
        else
        {
            glyph = *text++;
            uint8_t arg = *text++;
            count -= 2;
            size = g_user_glyph (x, y, glyph, arg);
        }
        unsigned delta = spacing + G_SIZE_W (size);
        x += delta;
        mw += delta;
        delta = G_SIZE_H (size);
        if (delta > mh)
            mh = delta;
    }

    return G_MK_SIZE (mw, mh);
}

uint32_t g_text (int x, int y, const uint8_t *text)
{
    return g_print (x, y, (int8_t)text [0], (unsigned)text [1] + 1, (const char *)(text + 2));
}

uint32_t g_printa (int x, int y, int spacing, unsigned count, const uint8_t *text, uint8_t glyph)
{
    if ((goc_glyph_types [glyph >> 3] & (1 << (glyph & 7))) == 0)
        return 0;

    unsigned mw = 0, mh = 0;
    const uint8_t *anim = goc_glyph (glyph);

    while (count--)
    {
        uint32_t size = g_anim (x, y, *text++, anim);
        unsigned delta = spacing + G_SIZE_W (size);
        x += delta;
        mw += delta;
        delta = G_SIZE_H (size);
        if (delta > mh)
            mh = delta;
    }

    return G_MK_SIZE (mw, mh);
}

uint32_t g_glyph_size (uint8_t glyph)
{
    const uint8_t *data = goc_glyph (glyph);
    if (goc_glyph_types [glyph >> 3] & (1 << (glyph & 7)))
    {
        unsigned nframes = (unsigned)data [1] + 1;
        unsigned delay = (unsigned)data [2] + 1;
        return g_anim_size ((clock / delay) % nframes, data);
    }

    return g_bitmap_size (data);
}

uint32_t g_print_size (int spacing, unsigned count, const char *text)
{
    unsigned mw = 0, mh = 0;
    while (count--)
    {
        uint8_t glyph = *text++;
        uint32_t size;
        if (glyph)
            size = g_glyph_size (glyph);
        else
        {
            glyph = *text++;
            uint8_t arg = *text++;
            count -= 2;
            size = g_user_glyph_size (glyph, arg);
        }
        unsigned delta = spacing + G_SIZE_W (size);
        mw += delta;
        delta = G_SIZE_H (size);
        if (delta > mh)
            mh = delta;
    }

    return G_MK_SIZE (mw, mh);
}

uint32_t g_printa_size (int spacing, unsigned count, const uint8_t *text, uint8_t glyph)
{
    if ((goc_glyph_types [glyph >> 3] & (1 << (glyph & 7))) == 0)
        return 0;

    unsigned mw = 0, mh = 0;
    const uint8_t *anim = goc_glyph (glyph);

    while (count--)
    {
        uint32_t size = g_anim_size (*text++, anim);
        unsigned delta = spacing + G_SIZE_W (size);
        mw += delta;
        delta = G_SIZE_H (size);
        if (delta > mh)
            mh = delta;
    }

    return G_MK_SIZE (mw, mh);
}
