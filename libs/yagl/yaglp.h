/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __YAGLP_H__
#define __YAGLP_H__

/// Set pixel (no clipping)
static inline void _pixel (unsigned x, unsigned y, unsigned v)
{
    unsigned ofs = G_FB_OFS (x, y);
    g_fb_t p = g.fb [ofs];
    p = (p & ~G_PM (x, y)) | G_PV (x, y, v);
    g.fb [ofs] = p;
}

#endif // __YAGLP_H__
