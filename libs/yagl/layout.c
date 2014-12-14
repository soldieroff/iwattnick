/*
    Yet Another Graphics Library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "yagl.h"
#include "glayout.h"
#include "glbc.h"
#include "usefun.h"

// x1,y1,x2,y2 can't be used initialized, but compiler doesn't understand that
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

void g_layout_draw (int x, int y, uint8_t layout)
{
    const uint8_t *data = goc_layout (layout);

    int w = uleb128 (&data);
    int h = uleb128 (&data);

    // clip everything to layout area
    g_rect_t old_clip = g.clip;
    g_clip_set_wh (x, y, w, h);

    for (;;)
    {
        uint8_t opcode = *data++;
        int x1, y1, x2, y2;

        switch (opcode & LOPA_MASK)
        {
            case LOPA_SSSS:
                x1 = x + sleb128 (&data);
                y1 = y + sleb128 (&data);
            case LOPA_SS:
                x2 = x + sleb128 (&data);
                y2 = y + sleb128 (&data);
                break;

            case LOPA_U:
                x1 = uleb128 (&data);
                break;

            default:
                break;
        }

        switch (opcode)
        {
            // Stop interpreting bytecode
            case LOP_END:
                goto leave;

            // (color code) set current drawing color
            case LOP_COLOR:
                g_color (x1);
                break;

            // (x, y) draw a dot
            case LOP_DOT:
                g_pixel (x2, y2);
                break;

            // (x1, y1, x2, y2) draw a line
            case LOP_LINE:
                g_line (x1, y1, x2, y2);
                break;

            // (x1, y1, x2, y2) draw a rectangle
            case LOP_RECT:
                g_rect (x1, y1, x2, y2);
                break;

            // (x1, y1, x2, y2) draw a filled box
            case LOP_BOX:
                g_box (x1, y1, x2, y2);
                break;

            // (x, y, layout) draw another layout
            case LOP_DRAW:
                g_layout_draw (x2, y2, *data++);
                break;

            // (x, y, text) draw a text string
            case LOP_TEXT:
                g_text (x2, y2, goc_text (*data++));
                break;

            // (x1, y1, x2, y2, menu) display a menu
            case LOP_MENU:
                g_menu_draw (x1, y1, x2, y2,
                    g_layouts [layout].menu = *data++);
                break;
        }
    }

leave:
    // Restore clipping rectangle
    g.clip = old_clip;
}
