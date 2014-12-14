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

/// The index of MENU objects (16-bit offsets relative to goc_menus)
extern const uint16_t goc_menu_index [];

/// MENU objects
extern const uint8_t goc_menus [];

/// The index of LAYOUT objects (16-bit offsets relative to goc_layouts)
extern const uint16_t goc_layout_index [];

/// LAYOUT objects
extern const uint8_t goc_layouts [];

/// Get a pointer to glyph data (BITMAP or ANIM) by glyph code
#define goc_glyph(x)	(goc_glyphs + goc_glyph_index [x])

/// Check if a glyph is animation or bitmap
#define goc_is_anim(x)	(goc_glyph_types [(x) >> 3] & (1 << ((x) & 7)))

/// Get a pointer to a text object, given a 16-bit offset
#define goc_text(x)	(goc_texts + x)

/// Get a pointer to a menu object by menu index
#define goc_menu(x)	(goc_menus + goc_menu_index [x])

/// Get a pointer to layout data by layout index
#define goc_layout(x)	(goc_layouts + goc_layout_index [x])

/**
 * This structure holds the state of a single menu
 */
typedef struct
{
    /// Active item index of the menu in this layout
    uint8_t item;
    /// The current Y scrolling speed
    int8_t yspeed;
    /// The current Y shift of the menu
    int16_t yshift;
} g_menu_t;

/**
 * The actions that can be applied on a menu
 */
typedef enum
{
    G_MENU_UP,
    G_MENU_DOWN,
    G_MENU_FIRST,
    G_MENU_LAST,
    G_MENU_ACTIVATE,
} g_menu_action_t;

/**
 * This struct holds the status of a single layout
 */
typedef struct
{
    /// The active menu index
    uint8_t menu;
    /// The layout that was active before this
    uint8_t prev;
} g_layout_t;

/// This array is defined in user code using the IMPLEMENT_LAYOUTS macro
extern g_layout_t g_layouts [];
/// This array is defined in user code using the IMPLEMENT_MENUS macro
extern g_menu_t g_menus [];

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
 *      User glyph code (usually one of VAR_xxx constants) in lower 8 bits,
 *      additional args may be present in bits 8-15
 * @return
 *      Glyph width in lower 16 bits, glyph height in upper 16 bits.
 */
extern uint32_t g_user_glyph (int x, int y, uint32_t glyph);

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
extern uint32_t g_print (int x, int y, int8_t spacing, unsigned count, const char *text);

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
extern uint32_t g_print_size (int8_t spacing, unsigned count, const char *text);

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
extern uint32_t g_printa (int x, int y, int8_t spacing, unsigned count, const char *text, uint8_t glyph);

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
extern uint32_t g_printa_size (int8_t spacing, unsigned count, const char *text, uint8_t glyph);

/**
 * Draw the layout referenced by index
 * @arg x
 *      The X coordinate of the top-left layout corner
 * @arg y
 *      The Y coordinate of the top-left layout corner
 * @arg layout
 *      The index of the layout to activate (one of the LAYOUT_xxx consts).
 */
extern void g_layout_draw (int x, int y, uint8_t layout);

/**
 * Draw a menu inside the given rectangle
 * @arg x1
 *      The X coordinate of the top-left corner of the menu area
 * @arg y1
 *      The Y coordinate of the top-left corner of the menu area
 * @arg x2
 *      The X coordinate of the bottom-right corner of the menu area
 * @arg y2
 *      The Y coordinate of the bottom-right corner of the menu area
 * @arg menu
 *      The menu object index (one of the MENU_xxx constants)
 */
extern void g_menu_draw (int x1, int y1, int x2, int y2, uint8_t menu);

/**
 * Move cursor in given menu. If moving past the first/last item the
 * cursor will wrap around to last/first item respectively.
 *
 * This function is usually invoked when user presses the cursor
 * movement keys.
 * @arg menu
 *      The menu object index (one of the MENU_xxx constants)
 * @arg action
 *      The action to execute on menu
 */
extern void g_menu_event (uint8_t menu, g_menu_action_t action);

/**
 * Execute a user action. This is usually called from
 * g_menu_event (..., G_MENU_ACTIVATE).
 * @arg action
 *      Action code (one of ACTION_xxx constants)
 * @arg args
 *      Additional action arguments as defined in goc file
 */
extern void g_user_action (unsigned action, int *args);

#endif // __GLAYOUT_H__
