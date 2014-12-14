/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __YAGLP_H__
#define __YAGLP_H__

#define SIZE_QUERY_Y 16384

#define VAR_dir		0
#define VAR_align	1

/// Set pixel (no clipping)
static inline void _pixel (unsigned x, unsigned y, unsigned v)
{
    unsigned ofs = G_FB_OFS (x, y);
    g_fb_t p = g.fb [ofs];
#if (G_BPP == 1)
    g_fb_t m = G_PV (x, y, 1);
    p &= ~m;
    if (v)
        p |= m;
#else
    p = (p & ~G_PM (x, y)) | G_PV (x, y, v);
#endif
    g.fb [ofs] = p;
}

/// Text cursor (caret)
typedef struct
{
    /// Caret X
    int16_t x;
    /// Caret Y
    int16_t y;
    /// Bounding box of all printed text
    g_rect_t b;
    /// Glyph spacing
    int8_t spacing;
    /// Text direction, size query flag
    uint8_t flags;
} g_caret_t;

#define G_DIR_RIGHT		0
#define G_DIR_DOWN		1

#define G_ALIGN_CENTER		0
#define G_ALIGN_RIGHT		1

#define G_FLAGS_DIRMASK		0x01
#define G_FLAGS_SIZEQ		0x80

/// Initialize the caret
extern void g_caret_init (g_caret_t *c, int x, int y, int8_t spacing);

/// Advance the caret after displaying a object of given size
extern void g_caret_advance (g_caret_t *c, uint32_t wh);

/// Get the size of the rectangle affected by printing
static inline uint32_t g_caret_size (g_caret_t *c)
{ return g_rect_size (&c->b); }

#endif // __YAGLP_H__
