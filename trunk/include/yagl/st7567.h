/*
    ST7567 LCD controller library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#ifndef __ST7567_H__
#define __ST7567_H__

#include "useful.h"

/**
 * @file st7567.h
 *      ST7567 LCD controller library.
 *      The library expects the following macros to be defined
 *      in your hardware.h file (example):
 *
 * @code
 * // Where we have connected the LCD based on the ST7567 controlled in SPI mode
 * #define ST7567_SPI_NUM		1
 * #define ST7567_SPI_TX_DMA_NUM	1
 * #define ST7567_SPI_TX_DMA_CHAN	3
 * #define ST7567_SPI_TX_DMA_IRQ_PRIO	0
 *
 * // Chip Select /CS
 * #define ST7567_CS_PORT		B
 * #define ST7567_CS_BIT		0
 * // Reset /RES
 * #define ST7567_RES_PORT		B
 * #define ST7567_RES_BIT		1
 * // Register Select RS (A0) (0 = instruction, 1 = data)
 * #define ST7567_RS_PORT		B
 * #define ST7567_RS_BIT		2
 * // Serial clock SCK
 * #define ST7567_SCK_PORT		A
 * #define ST7567_SCK_BIT		5
 * // Serial data SDA (MOSI)
 * #define ST7567_SDA_PORT		A
 * #define ST7567_SDA_BIT		7
 * @endcode
 */

/// Tell YAGL how many bits per pixel ST7567 uses
#define G_BPP			1
/// Tell YAGL the code of bit ordering we use in framebuffer
#define G_BORD			G_BORD_VLSBT

/// Enable or disable display
#define ST7567_DISP_ENABLE(d)	((d) ? 0xAF : 0xAE)
/// Set display start scanline (0-63)
#define ST7567_START_LINE(l)	(0x40 | ((l) & 63))
/// Set page number to write into (0-15)
#define ST7567_SET_PAGE(y)	(0xB0 | ((y) & 15))
/// Set column address lower 4 bits (n=0) or upper 4 bits (n=1)
#define ST7567_SET_COLUMN(n,x)	(((n) ? 0x10 : 0x00) | (((x) >> ((n) ? 4 : 0)) & 15))
/// Set SEG direction
#define ST7567_HORIZ_DIR(d)	((d) ? 0xA1 : 0xA0)
/// Set COM direction
#define ST7567_VERT_DIR(d)	((d) ? 0xC8 : 0xC0)
/// Invert display image
#define ST7567_INVERT(b)	((b) ? 0xA7 : 0xA6)
/// Light all pixels on display
#define ST7567_ALL_PIXELS_ON(b)	((b) ? 0xA5 : 0xA4)
/// Select bias setting 0=1/9; 1=1/7 (at 1/65 duty)
#define ST7567_SET_BIAS(b)	((b) ? 0xA3 : 0xA2)
/// Software reset of the controller
#define ST7567_RESET		0xE2
/// Set internal voltage booster, regulator, follower states
#define ST7567_PWR_CTRL(vb,vr,vf) \
	(0x28 | ((vb) ? 4 : 0) | ((vr) ? 2 : 0) | ((vf) ? 1 : 0))
/// Select regulation resistor ratio (0-7), rough contrast
#define ST7567_CONTRAST_R(r)	(0x20 | ((r) & 7))
/// Set electronic volume (EV) level (fine contrast), 2-byte command
#define ST7567_CONTRAST_F(x)		0x81,((x) & 31)
/// Set booster level: 00=4X, 01=5X, 10=6X
#define ST7567_BOOSTER(l)	0xF8,((l) & 3)
/// No operation
#define ST7567_NOP		0xE3
/// Show cursor (undocumented?)
#define ST7567_CURSOR_ON	0xAD,0x00
/// Hide cursor (undocumented)
#define ST7567_CURSOR_OFF	0xAC
/// ??? Advanced Control
#define ST7567_APC0		0xFA,0x10

/**
 * Initialize the ST7567 controller.
 * Uses ST7567_XXX macros from your hardware.h file.
 */
extern void st7567_init ();

/**
 * Put a sequence of commands or data bytes into transmission queue.
 * The bytes will be asynchronously transmitted to LCD.
 * The command sequence must stay unchanged until the command
 * sequence is transmitted.
 * @arg command
 *      true if @a data points to controller commands, false if
 *      it points to display RAM data.
 * @arg data
 *      A pointer to a command sequence or data bytes
 * @arg len
 *      Number of bytes in sequence
 * @return
 *      true on success, false if controller is busy
 */
extern bool st7567_send (bool command, const uint8_t *data, unsigned len);

/**
 * Check if the ST7567 controller is idle.
 * This can be used to check if transmission of data sent to LCD with
 * st7567_send() is finished.
 */
extern bool st7567_idle ();

/**
 * Send the whole framebuffer to the device.
 */
extern void st7567_refresh ();

// --- // Aliases // --- //

#define g_refresh st7567_refresh

#endif // __ST7567_H__
