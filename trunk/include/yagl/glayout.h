/*
    Yet Another Graphics library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#ifndef __GLAYOUT_H__
#define __GLAYOUT_H__

/**
 * @file glayout.h
 *      This library helps to organize a simple user interface meant for
 *      simple devices. The main idea is to have a number of "layouts",
 *      every layout consists of a number of static graphics objects
 *      (lines, rectangles, texts and so on), and a number of menu items.
 *      Every menu item has an associated action.
 *
 *      A layout may be fullscreen and partial screen; in the second case
 *      it is displayed over the previous layout.
 */

#include <stdint.h>

/// Glyph data 16-bit offsets (relative to goc_glyphs)
extern const uint16_t goc_glyph_index [];

/// Glyph bitmaps/anims
extern const uint8_t goc_glyphs [];

/// Glyph type bits (BITMAP if bit is 0, ANIM if bit is 1)
extern const uint8_t goc_glyph_types [];

/// TEXT objects
extern const uint8_t goc_texts [];

/// MENU objects
extern const uint8_t goc_menus [];

/// LAYOUT objects
extern const uint8_t goc_layouts [];

/// Get a pointer to glyph data (BITMAP or ANIM) by glyph code
#define goc_glyph(x)	(goc_glyphs + goc_glyph_index [x])

/**
 * This struct holds the status of a single layout
 */
typedef struct
{
    /// The layout that was active before this
    uint8_t prev;
} g_layout_t;

/**
 * Display a glyph at given position. A glyph may be either
 * a bitmap or an anim, the type is automatically detected.
 * The animation frame to show is deducted from the system
 * clock variable.
 * @arg x
 *      The X coordinate of the glyph
 * @arg y
 *      The Y coordinate of the glyph
 * @arg glyph
 *      Glyph code
 * @return
 *      Glyph size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_glyph (int x, int y, uint8_t glyph);

/**
 * Query the size of a glyph
 * @arg glyph
 *      Glyph code
 * @return
 *      Glyph size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_glyph_size (uint8_t glyph);

/**
 * User function to display a custom "glyph" (usually a variable,
 * but actually can be anything). This function must be defined in user code.
 * @arg x
 *      The X coordinate of the user glyph
 * @arg y
 *      The Y coordinate of the user glyph
 * @arg glyph
 *      User glyph code (usually one of VAR_xxx constants)
 * @arg arg
 *      Additional attribute (specified after comma in goc file)
 * @return
 *      Glyph width in lower 16 bits, glyph height in upper 16 bits.
 */
extern uint32_t g_user_glyph (int x, int y, uint8_t glyph, uint8_t arg);

/**
 * Query the size of a custom "glyph"
 * @arg glyph
 *      User glyph code (usually one of VAR_xxx constants)
 * @arg arg
 *      Additional attribute (specified after comma in goc file)
 * @return
 *      Glyph width in lower 16 bits, glyph height in upper 16 bits.
 */
extern uint32_t g_user_glyph_size (uint8_t glyph, uint8_t arg);

/**
 * Display a text object.
 * @arg x
 *      The X coordinate of the text
 * @arg y
 *      The Y coordinate of the text
 * @arg text
 *      A pointer to text object data. First byte contains glyph spacing,
 *      then comes the number of glyphs, and then - glyph codes.
 * @return
 *      Resulting text size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_text (int x, int y, const uint8_t *text);

/**
 * Display a text string. This is a lower-level routine, compared to g_text,
 * and could be helpful to display arbitrary text from the program (not
 * predefined strings, like g_text).
 * @arg x
 *      The X coordinate of the text
 * @arg y
 *      The Y coordinate of the text
 * @arg spacing
 *      Glyph spacing in pixels
 * @arg count
 *      Number of characters to display
 * @arg text
 *      A pointer to text object data. First byte contains glyph spacing,
 *      then comes the number of glyphs, and then - glyph codes.
 * @return
 *      Resulting text size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_print (int x, int y, int spacing, unsigned count, const char *text);

/**
 * Query the size of the text as if displayed by g_print().
 * @arg spacing
 *      Glyph spacing in pixels
 * @arg count
 *      Number of characters to display
 * @arg text
 *      A pointer to text object data. First byte contains glyph spacing,
 *      then comes the number of glyphs, and then - glyph codes.
 * @return
 *      Resulting text size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_print_size (int spacing, unsigned count, const char *text);

/**
 * Display a text string using a single animation as a source of glyph images.
 * This is handy to have multiple fonts, for example various size digits etc.
 * The animation contains the glyphs as animation frames, and the 'text' contains
 * frame numbers.
 * @arg x
 *      The X coordinate of the text
 * @arg y
 *      The Y coordinate of the text
 * @arg spacing
 *      Glyph spacing in pixels
 * @arg count
 *      Number of characters to display
 * @arg text
 *      A pointer to text object data. First byte contains glyph spacing,
 *      then comes the number of glyphs, and then - glyph codes.
 * @arg glyph
 *      Animation glyph code.
 * @return
 *      Resulting text size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_printa (int x, int y, int spacing, unsigned count, const uint8_t *text, uint8_t glyph);

/**
 * Query the size of the text as if displayed by g_printa().
 * @arg spacing
 *      Glyph spacing in pixels
 * @arg count
 *      Number of characters to display
 * @arg text
 *      A pointer to text object data. First byte contains glyph spacing,
 *      then comes the number of glyphs, and then - glyph codes.
 * @arg glyph
 *      Animation glyph code.
 * @return
 *      Resulting text size, top 16 bits = height, low 16 bits = width
 */
extern uint32_t g_printa_size (int spacing, unsigned count, const uint8_t *text, uint8_t glyph);

#endif // __GLAYOUT_H__
