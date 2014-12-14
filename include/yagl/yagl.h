/*
    Yet Another Graphics library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#ifndef __YAGL_H__
#define __YAGL_H__

/**
 * @file yagl.h
 *      The Yet Another Graphics Library is a lightweight drawing
 *      library optimized for 32-bit microcontrollers. The library
 *      provides routines for drawing some graphics primitives
 *      and has support for clipping to rectangular screen regions.
 *
 *      It has built-in support for some simple LCD controllers.
 */

/// Define if you prefer speed over code size
#define G_OPTIMIZE_SPEED

/**
 * Bytes are vertical strips of pixels, LSB at top, strips ordered horizontally,
 * then another block of vertical strips of pixels.
 *
 * @verbatim
 * ofs  0 1 2 3 4 ...
 * bit
 *  0   0 0 0 1 1
 *  1   0 0 1 0 1
 *  2   0 1 0 0 1
 *  3   1 0 0 0 1
 *  4   1 0 0 0 1
 *  5   1 1 1 1 1
 *  6   1 0 0 0 1
 *  7   1 0 0 0 1
 *
 * ofs  W W+1 W+2 W+3 W+4 ...
 * ofs  2*W 2*W+1 2*W+2 2*W+3 2*W+4 ...
 * @endverbatim
 */
#define G_BORD_VLSBT		1000

#include G_HARDWARE_H

/**
 * @file yagl.h
 *      Main header file for Yet Another Graphics Library.
 *      YAGL is a simple microcontroller-oriented graphics library for
 *      simple LCD controllers/screens.
 *
 *      You must define the following macros in your hardware
 *      definitions file:
 *
 *      @li G_HARDWARE_H defines the header file for the controller
 *          that you want to use, e.g. "st7567.h".
 *      @li G_FB_W defines the width of the framebuffer, in pixels.
 *      @li G_FB_H defines the height of the framebuffer, in pixels.
 *
 *      The following macros are usually defined in controller-specific
 *      header file:
 *
 *      @li G_BPP defines the number of bits per pixel
 *      @li G_BORD defines the bit ordering code (see G_BORD_XXX)
 */

#if G_BORD == G_BORD_VLSBT
/// The type of framebuffer elements
typedef uint8_t g_fb_t;
/// sizeof (g_fb_t)*8, for preprocessor spells, must divide evenly by G_BPP
#  define G_FB_T_BITS		8
/// Pixel value
#  define G_PV(x,y,v)		((v) << (((y) & (8/G_BPP - 1)) * G_BPP))
/// Pixel mask
#  define G_PM(x,y)		G_PV (x, y, (1 << G_BPP) - 1)
/// Framebuffer size
#  define G_FB_SIZE		(G_FB_W * ((G_FB_H + (8/G_BPP) - 1) / (8/G_BPP)))
/// Get the framebuffer offset of a pixel
#  define G_FB_OFS(x,y)		((y / (8/G_BPP)) * G_FB_W + x)
#else
#  error "Unknown bit ordering"
#endif

/// Pixels in g_fb_t
#define G_FB_T_PIX		(G_FB_T_BITS / G_BPP)

#if G_BORD == G_BORD_VLSBT

#if (G_FB_T_BITS / G_BPP) == 8
#define G_ALLPIX(x) \
        ({ g_fb_t _2 = G_PV (0, 0, x) | G_PV (0, 1, x); \
           g_fb_t _4 = _2 | (_2 << 2*G_BPP); \
           g_fb_t _8 = _4 | (_4 << 4*G_BPP); \
           _8; })
#elif (G_FB_T_BITS / G_BPP) == 4
#define G_ALLPIX(x) \
        ({ g_fb_t _2 = G_PV (0, 0, x) | G_PV (0, 1, x); \
           g_fb_t _4 = _2 | (_2 << 2*G_BPP); \
           _4; })
#elif (G_FB_T_BITS / G_BPP) == 2
#define G_ALLPIX(x) \
        (G_PV (0, 0, x) | \
         G_PV (0, 1, x))
#elif (G_FB_T_BITS / G_BPP) == 1
#define G_ALLPIX(x) \
        G_PV (0, 0, x)
#endif

#endif

/// Create a single 'size' value from width and height
#define G_MK_SIZE(w,h)	(((w) & 0xffff) | ((h) << 16))
/// Extract width from compound 'size'
#define G_SIZE_W(x)	((x) & 0xffff)
/// Extract height from compound 'size'
#define G_SIZE_H(x)	((x) >> 16)

/**
 * A clipping rectangle
 */
typedef struct
{
    int16_t xmin;
    int16_t ymin;
    int16_t xmax;
    int16_t ymax;
} g_rect_t;

/**
 * All graphics library variables
 */
typedef struct
{
    /// Clipping rectangle
    g_rect_t clip;

    /// Drawing color
    g_fb_t color;

    /// The framebuffer (G_FB_W x G_FB_H bits)
    g_fb_t fb [G_FB_SIZE];
} g_t;

/// The static graphics library variables
extern g_t g;

/**
 * The system clock variable - used for animations.
 * WARNING! It is insistently recommended not to change this variable from
 * interrupt handlers. Instead, at the start of drawing copy the system
 * clock variable here.
 */
extern uint32_t g_clock;

/**
 * Initialize the graphics library
 */
extern void g_init ();

/**
 * Clear the framebuffer. This function ignores current color,
 * and ignores clipping rectangle.
 */
extern void g_clear ();

/**
 * Set clipping area to whole screen
 */
extern void g_clip_screen ();

/**
 * Set the clipping rectangle. The clipping rectangle will be adjusted
 * so that it completely lies inside the screen area.
 * @arg xmin
 *      The minimal X clipping coordinate (inclusive)
 * @arg ymin
 *      The minimal Y clipping coordinate (inclusive)
 * @arg xmax
 *      The maximal X clipping coordinate (inclusive)
 * @arg ymax
 *      The maximal Y clipping coordinate (inclusive)
 */
extern void g_clip_set (uint16_t xmin, uint16_t ymin, uint16_t xmax, uint16_t ymax);

/**
 * Set the clipping rectangle. The clipping rectangle will be adjusted
 * so that it completely lies inside the screen area.
 * @arg x
 *      The minimal X clipping coordinate (inclusive)
 * @arg y
 *      The minimal Y clipping coordinate (inclusive)
 * @arg w
 *      The clipping area width
 * @arg h
 *      The clipping area height
 */
extern void g_clip_set_wh (uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * Adjust the clipping rectangle so that it doesn't lie outside
 * the screen. This function is automatically called from the g_clip_set*
 * functions, so you don't have to call it unless you change the clipping
 * rectangle manually.
 */
extern void g_clip_limit ();

/**
 * Set current color
 * @arg c
 *      The color used for drawing
 */
static inline void g_color (g_fb_t c)
{ g.color = c & ((1 << G_BPP) - 1); }

/**
 * Display a single pixel with current color
 * @arg x
 *      Pixel X coordinate
 * @arg y
 *      Pixel Y coordinate
 */
extern void g_pixel (int x, int y);

/**
 * Draw an arbitrary line with current color
 * @arg x1
 *      The first X coordinate
 * @arg y1
 *      The first Y coordinate
 * @arg x2
 *      The second X coordinate
 * @arg y2
 *      The second Y coordinate
 */
extern void g_line (int x1, int y1, int x2, int y2);

/**
 * Draw horizontal line with current color
 * @arg x1
 *      First X coordinate
 * @arg x2
 *      Last X coordinate
 * @arg y
 *      The Y coordinate of the line
 */
extern void g_hline (int x1, int x2, int y);

/**
 * Draw vertical line with current color
 * @arg x
 *      The X coordinate of the line
 * @arg y1
 *      First Y coordinate
 * @arg y2
 *      Last Y coordinate
 */
extern void g_vline (int x, int y1, int y2);

/**
 * Draw a rectangle with current color
 * @arg x1
 *      The first X coordinate
 * @arg y1
 *      The first Y coordinate
 * @arg x2
 *      The second X coordinate
 * @arg y2
 *      The second Y coordinate
 * @arg c
 *      Rectangle color
 */
extern void g_rect (int x1, int y1, int x2, int y2);

/**
 * Draw a filled box with current color
 * @arg x1
 *      The first X coordinate
 * @arg y1
 *      The first Y coordinate
 * @arg x2
 *      The second X coordinate
 * @arg y2
 *      The second Y coordinate
 */
extern void g_box (int x1, int y1, int x2, int y2);

/**
 * Display a monochrome bitmap.
 *
 * The order of bits and bytes in bitmap is supposed
 * to be the same as in framebuffer.
 * @arg x
 *      The X coordinate of the bitmap
 * @arg y
 *      The Y coordinate of the bitmap
 * @arg bitmap
 *      A pointer to the bitmap. The first byte is hhwwwwww
 *      bit format, height is (hh+1)*8, width is wwwwww+1,
 *      e.g. bitmaps can be up to 64x32 pixels size.
 * @return
 *      Bitmap width in lower 16 bits, height in upper 16 bits.
 */
extern uint32_t g_bitmap (int x, int y, const uint8_t *bitmap);

/**
 * Query the size of a bitmap
 * @arg bitmap
 *      A pointer to the bitmap. The first byte is hhwwwwww
 *      bit format, height is (hh+1)*8, width is wwwwww+1,
 *      e.g. bitmaps can be up to 64x32 pixels size.
 * @return
 *      Bitmap width in lower 16 bits, height in upper 16 bits.
 */
extern uint32_t g_bitmap_size (const uint8_t *bitmap);

/// Query the maximal size of all animation frames
#define g_anim_maxsize g_bitmap_size

/**
 * Display animation frame.
 *
 * @arg x
 *      The X coordinate of the animation
 * @arg y
 *      The Y coordinate of the animation
 * @arg n
 *      Frame number to display
 * @arg anim
 *      A pointer to animation data. The first byte contains
 *      maximal frame width/height exactly like a bitmap.
 *      Second byte is number of frames, third is animation
 *      delay, then first frame bitmap, second, third and so on.
 * @return
 *      Frame width in lower 16 bits, height in upper 16 bits.
 */
extern uint32_t g_anim (int x, int y, unsigned n, const uint8_t *anim);

/**
 * Query the size of an animation frame.
 * @arg n
 *      Frame number to query
 * @arg anim
 *      A pointer to animation data. The first byte contains
 *      maximal frame width/height exactly like a bitmap.
 *      Second byte is number of frames, third is animation
 *      delay, then first frame bitmap, second, third and so on.
 * @return
 *      Frame width in lower 16 bits, height in upper 16 bits.
 */
extern uint32_t g_anim_size (unsigned n, const uint8_t *anim);

/**
 * Initialize a rect structure to a empty rectangle at given position
 * @arg r
 *      The rectangle
 * @arg x
 *      The X coordinate
 * @arg y
 *      The Y coordinate
 */
extern void g_rect_init (g_rect_t *r, int x, int y);

/**
 * Extend the rectangle to include given pixel
 * @arg r
 *      The rectangle
 * @arg x
 *      The X coordinate
 * @arg y
 *      The Y coordinate
 */
extern void g_rect_extend (g_rect_t *r, int x, int y);

/**
 * Query the size of a rectangle
 * @arg r
 *      The rectangle
 * @return
 *      Frame width in lower 16 bits, height in upper 16 bits.
 */
extern uint32_t g_rect_size (g_rect_t *r);

/**
 * Set the boundaries of given rectangle
 * @arg x
 *      The X coordinates of the top-left rectangle corner
 * @arg y
 *      The Y coordinates of the top-left rectangle corner
 * @arg w
 *      The width of the rectangle
 * @arg h
 *      The height of the rectangle
 */
extern void g_rect_set_wh (g_rect_t *r, int x, int y, int w, int h);

#endif // __YAGL_H__
