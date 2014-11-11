/*
    Miscelaneous function for embedded systems
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __GEARS_H__
#define __GEARS_H__

/**
 * @file gears.h
 *      This file defines the interface to a number of useful functions.
 *      Many of them are libc-like, but were optimized for size.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Fill the first @a len bytes of the memory area pointed to by
 * @a dest with the constant byte @a c.
 * @arg dest
 *      A pointer of memory to fill
 * @arg c
 *      The value to fill with
 * @arg len
 *      Number of bytes to fill
 */
extern void memset (void *dest, char c, unsigned len);

/**
 * Fill the first @a len bytes of the memory area pointed to by
 * @a dest with the zero constant.
 * @arg dest
 *      A pointer of memory to fill
 * @arg len
 *      Number of bytes to fill
 */
extern void memclr (void *dest, unsigned len);

/**
 * Optimized traditional memcpy().
 * @arg dest
 *      The destination pointer
 * @arg src
 *      Source pointer
 * @arg len
 *      Number of bytes to copy
 */
extern void memcpy (void *dest, const void *src, unsigned len);

#ifdef __cplusplus
}
#endif

#endif // __GEARS_H__
