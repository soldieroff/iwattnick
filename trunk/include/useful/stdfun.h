/*
    Useful function for embedded systems
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __STDFUN_H__
#define __STDFUN_H__

/**
 * @file stdfun.h
 *      A number of useful functions, balancedly optimized for speed/size.
 *      Some functions are like their ANSI C counterparts, but they aren't
 *      meant to be 100% compliant with the standard.
 */

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

/**
 * Set the starting point for rand().
 * @arg seed
 *      The number that determines the pseudo-random sequence.
 */
extern void srand (unsigned seed);

/**
 * Get a pseudo-random number.
 * @return
 *      A new random number in the range 0..MAX_UNSIGNED_INT
 */
extern unsigned rand ();

#endif // __STDFUN_H__
