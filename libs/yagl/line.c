/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "yaglp.h"
#include "useful.h"

/* Clipping algorithm as per "A Line Segment Clipping Algorithm in 2D"
 * by Bimal Kumar Ray, International Journal of Computer Graphics
 * Vol. 3, No. 2, November, 2012
 */
static bool g_clip_one (int *x, int *y, int dx, int dy)
{
    if ((*x >= g.clip.xmin) &&
        (*x <= g.clip.xmax) &&
        (*y >= g.clip.ymin) &&
        (*y <= g.clip.ymax))
        return true;

    if (*x < g.clip.xmin)
    {
        if ((dx * (g.clip.ymin - *y) - dy * (g.clip.xmin - *x)) *
            (dx * (g.clip.ymax - *y) - dy * (g.clip.xmin - *x)) <= 0)
        {
            *y += ((g.clip.xmin - *x) * dy) / dx;
            *x = g.clip.xmin;
            return true;
        }
    }
    else if (*x > g.clip.xmax)
    {
        if ((dx * (g.clip.ymin - *y) - dy * (g.clip.xmax - *x)) *
            (dx * (g.clip.ymax - *y) - dy * (g.clip.xmax - *x)) <= 0)
        {
            *y += ((g.clip.xmax - *x) * dy) / dx;
            *x = g.clip.xmax;
            return true;
        }
    }

    if (*y < g.clip.ymin)
    {
        if ((dx * (g.clip.ymin - *y) - dy * (g.clip.xmin - *x)) *
            (dx * (g.clip.ymin - *y) - dy * (g.clip.xmax - *x)) <= 0)
        {
            *x += ((g.clip.ymin - *y) * dx) / dy;
            *y = g.clip.ymin;
            return true;
        }
    }
    else if (*y > g.clip.ymax)
    {
        if ((dx * (g.clip.ymax - *y) - dy * (g.clip.xmin - *x)) *
            (dx * (g.clip.ymax - *y) - dy * (g.clip.xmax - *x)) <= 0)
        {
            *x += ((g.clip.ymax - *y) * dx) / dy;
            *y = g.clip.ymax;
            return true;
        }
    }

    return false;
}

/* For better clipping precision it would be better to switch from
 * integer coordinates to 1.15.16 fixed-point from the beginning
 * of the function, but that would make the code less readable and
 * a bit larger.
 */
void g_line (int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    if (dx == 0)
    {
        g_vline (x1, y1, y2);
        return;
    }

    int dy = y2 - y1;
    if (dy == 0)
    {
        g_hline (x1, x2, y1);
        return;
    }

    // Trivial clipping tests
    if ((x1 < g.clip.xmin) && (x2 < g.clip.xmin))
        return;
    if ((x1 > g.clip.xmax) && (x2 > g.clip.xmax))
        return;
    if ((y1 < g.clip.ymin) && (y2 < g.clip.ymin))
        return;
    if ((y1 > g.clip.ymax) && (y2 > g.clip.ymax))
        return;

    if (!g_clip_one (&x1, &y1, dx, dy) ||
        !g_clip_one (&x2, &y2, dx, dy))
        return;

    if (ABS (dx) < ABS (dy))
    {
        // Go vertically
        if (y1 > y2)
        {
            XCHG (x1, x2);
            XCHG (y1, y2);
        }

        dx = ((x2 - x1) << 16) / (y2 - y1);
        x1 = (x1 << 16) | 0x8000;

        while (y1 <= y2)
        {
            _pixel (x1 >> 16, y1, g.color);
            x1 += dx;
            y1++;
        }
    }
    else
    {
        // Go horizontally
        if (x1 > x2)
        {
            XCHG (x1, x2);
            XCHG (y1, y2);
        }

        dy = ((y2 - y1) << 16) / (x2 - x1);
        y1 = (y1 << 16) | 0x8000;

        while (x1 <= x2)
        {
            _pixel (x1, y1 >> 16, g.color);
            x1++;
            y1 += dy;
        }
    }
}

void g_hline (int x1, int x2, int y)
{
    if ((y < g.clip.ymin) ||
        (y > g.clip.ymax))
        return;

    if (x1 > x2)
        XCHG (x1, x2);

    if (x1 < g.clip.xmin)
        x1 = g.clip.xmin;
    if (x2 > g.clip.xmax)
        x2 = g.clip.xmax;

    if (x1 > x2)
        return;

    while (x1 <= x2)
    {
        _pixel (x1, y, g.color);
        x1++;
    }
}

void g_vline (int x, int y1, int y2)
{
    if ((x < g.clip.xmin) ||
        (x > g.clip.xmax))
        return;

    if (y1 > y2)
        XCHG (y1, y2);

    if (y1 < g.clip.ymin)
        y1 = g.clip.ymin;
    if (y2 > g.clip.ymax)
        y2 = g.clip.ymax;

    if (y1 > y2)
        return;

#if defined (G_OPTIMIZE_SPEED) && (G_BORD == G_BORD_VLSBT)

    unsigned sofs = G_FB_OFS (x, y1);
    unsigned eofs = G_FB_OFS (x, y2);

    if (sofs > eofs)
        return;

    g_fb_t pixels = G_ALLPIX (g.color);

    unsigned dy = y1 & (G_FB_T_PIX - 1);
    if (dy > 0)
    {
        g_fb_t mask = G_ALLPIX ((1 << G_BPP) - 1) &
            (((1 << G_FB_T_BITS) - 1) << (dy * G_BPP));

        if (sofs == eofs)
        {
            dy = y2 & (G_FB_T_PIX - 1);
            mask &= (((1 << G_FB_T_BITS) - 1) >> ((G_FB_T_PIX - 1 - dy) * G_BPP));
        }

        g.fb [sofs] = (g.fb [sofs] & ~mask) | (pixels & mask);

        if (sofs == eofs)
            return;

        sofs += G_FB_W;
    }

    while (sofs < eofs)
    {
        g.fb [sofs] = pixels;
        sofs += G_FB_W;
    }

    if (sofs == eofs)
    {
        dy = y2 & (G_FB_T_PIX - 1);
        g_fb_t mask = G_ALLPIX ((1 << G_BPP) - 1) &
            (((1 << G_FB_T_BITS) - 1) >> ((G_FB_T_PIX - 1 - dy) * G_BPP));
        g.fb [sofs] = (g.fb [sofs] & ~mask) | (pixels & mask);
    }

#else

    while (y1 <= y2)
    {
        _pixel (x, y1, g.color);
        y1++;
    }

#endif
}
