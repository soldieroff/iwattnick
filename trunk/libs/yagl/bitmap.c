/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "yaglp.h"

uint32_t g_bitmap (int x, int y, const uint8_t *bitmap)
{
    unsigned w = *bitmap;
    unsigned h = ((w & 0xc0) >> 3) + 8;
    w = (w & 0x3f) + 1;
    uint32_t r = G_MK_SIZE (w, h);

    if (x > g.clip.xmax)
        return r;

    if (x < g.clip.xmin)
    {
        x = (g.clip.xmin - x);
        if (x < 0)
            return r;
        w -= x;
        bitmap += (h >> 3) * x;
        x = g.clip.xmin;
    }

#if G_BORD == G_BORD_VLSBT
    while (w)
    {
        h |= h << 8;
        int yy = y;
        while (h & 0xff00)
        {
            unsigned bits = *++bitmap;
            h |= 0x080000;
            while (bits && (h & 0xFF0000))
            {
                if (yy > g.clip.ymax)
                    break;
                if ((bits & 1)
                 && (yy >= g.clip.ymin))
                    _pixel (x, yy, g.color);

                yy++;
                h -= 0x010000;
                bits >>= 1;
            }
            yy += (h >> 16);
            h = (h & 0xffff) - 0x0800;
        }

        x++;
        w--;
    }
#else
#  error "Not implemented for current framebuffer format"
#endif

    return r;
}

uint32_t g_anim (int x, int y, unsigned n, const uint8_t *anim)
{
    if (n > anim [1])
        return 0;

    anim += 3;
    while (n--)
    {
        unsigned wh = *anim++;
        anim += (((wh & 0xc0) >> 6) + 1) * ((wh & 0x3f) + 1);
    }

    return g_bitmap (x, y, anim);
}

uint32_t g_bitmap_size (const uint8_t *bitmap)
{
    unsigned w = *bitmap;
    unsigned h = ((w & 0xc0) >> 3) + 8;
    w = (w & 0x3f) + 1;
    return G_MK_SIZE (w, h);
}

uint32_t g_anim_size (unsigned n, const uint8_t *anim)
{
    if (n > anim [1])
        return 0;

    anim += 3;
    while (n--)
    {
        unsigned wh = *anim++;
        anim += (((wh & 0xc0) >> 6) + 1) * ((wh & 0x3f) + 1);
    }

    return g_bitmap_size (anim);
}
