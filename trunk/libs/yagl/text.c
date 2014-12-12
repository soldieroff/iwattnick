/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "glayout.h"
#include "yaglp.h"

#define SIZE_QUERY_Y 16384

uint32_t g_glyph (int x, int y, uint8_t glyph)
{
    const uint8_t *data = goc_glyph (glyph);
    if (goc_is_anim (glyph))
    {
        unsigned nframes = (unsigned)data [1] + 1;
        unsigned delay = (unsigned)data [2] + 1;
        nframes = (g_clock / delay) % nframes;

        if (y == SIZE_QUERY_Y)
            return g_anim_size (nframes, data);
        else
            return g_anim (x, y, nframes, data);
    }

    if (y == SIZE_QUERY_Y)
        return g_bitmap_size (data);
    else
        return g_bitmap (x, y, data);
}

uint32_t g_print (int x, int y, int8_t spacing, unsigned count, const char *text)
{
    g_caret_t caret;
    g_caret_init (&caret, x, y, spacing);

    if (y == SIZE_QUERY_Y)
        caret.flags |= G_FLAGS_SIZEQ;

    while (count)
    {
        uint32_t glyph = *text++;
        count--;

        uint32_t size;

        if (glyph)
        {
            if (caret.flags & G_FLAGS_SIZEQ)
                size = g_glyph_size (glyph);
            else
                size = g_glyph (caret.x, caret.y, glyph);
        }
        else
        {
            glyph = *(uint16_t *)text;
            text += 2;
            count -= 2;

            switch (glyph & 0xff)
            {
                case VAR_dir:
                    caret.flags = (caret.flags & ~G_FLAGS_DIRMASK) | ((glyph >> 8) & G_FLAGS_DIRMASK);
                    continue;

                case VAR_align:
                    // don't recurse when querying size
                    if (caret.flags & G_FLAGS_SIZEQ)
                        goto leave;

                    size = g_print_size (caret.spacing, count, text);
                    switch (glyph >> 8)
                    {
                        case G_ALIGN_CENTER:
                            caret.x += (g.clip.xmax + 1 - caret.x - G_SIZE_W (size)) / 2;
                            break;

                        case G_ALIGN_RIGHT:
                            caret.x = (g.clip.xmax + 1 - G_SIZE_W (size));
                            break;
                    }
                    continue;

                default:
                    size = g_user_glyph (caret.x, caret.y, glyph);
                    break;
            }
        }

        g_caret_advance (&caret, size);
    }

leave:
    return g_caret_size (&caret);
}

uint32_t g_text (int x, int y, const uint8_t *text)
{
    return g_print (x, y, (int8_t)text [0], (unsigned)text [1] + 1, (const char *)(text + 2));
}

uint32_t g_printa (int x, int y, int8_t spacing, unsigned count, const char *text, uint8_t glyph)
{
    if (!goc_is_anim (glyph))
        return 0;

    const uint8_t *anim = goc_glyph (glyph);
    g_caret_t caret;
    g_caret_init (&caret, x, y, spacing);

    if (y == SIZE_QUERY_Y)
        caret.flags |= G_FLAGS_SIZEQ;

    while (count--)
    {
        uint32_t size;
        if (caret.flags & G_FLAGS_SIZEQ)
            size = g_anim_size (*text++, anim);
        else
            size = g_anim (caret.x, caret.y, *text++, anim);

        g_caret_advance (&caret, size);
    }

    return g_caret_size (&caret);
}

uint32_t g_glyph_size (uint8_t glyph)
{
    return g_glyph (SIZE_QUERY_Y, SIZE_QUERY_Y, glyph);
}

uint32_t g_print_size (int8_t spacing, unsigned count, const char *text)
{
    return g_print (SIZE_QUERY_Y, SIZE_QUERY_Y, spacing, count, text);
}

uint32_t g_printa_size (int8_t spacing, unsigned count, const char *text, uint8_t glyph)
{
    return g_printa (SIZE_QUERY_Y, SIZE_QUERY_Y, spacing, count, text, glyph);
}
