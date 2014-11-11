/*
    Useful function for embedded systems
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __USEFUL_H__
#define __USEFUL_H__

/**
 * @file gears.h
 *      The gears library provides a number of useful functions,
 *      balancedly optimized for speed/size. Some functions are
 *      like their ANSI C counterparts, but they aren't meant
 *      to be 100% compliant with the standard.
 */

#include <stdint.h>

/// Get the number of elements in a static array
#define ARRAY_LEN(x)		(sizeof (x) / sizeof (x [0]))

/// Structure attribute to avoid aligning structure members
#define PACKED			__attribute__ ((packed))

/// Get the offset of member m in compound type t
#define OFFSETOF(t, m)		__builtin_offsetof (t, m)

/// Exchange two values of the same type
#define XCHG(x, y)		{ typeof (x) __z = x; x = y; y = __z; }

#if !defined __cplusplus
/// Define the bool type for C
typedef enum { false = 0, true = !false } bool;
#endif

#define _JOIN2(a,b)		a##b
/// Join two tokens together and interpret the result as a new token
#define JOIN2(a,b)		_JOIN2(a, b)

#define _JOIN3(a,b,c)		a##b##c
/// Join three tokens together and interpret the result as a new token
#define JOIN3(a,b,c)		_JOIN3(a,b,c)

#define _JOIN4(a,b,c,d)		a##b##c##d
/// Join three tokens together and interpret the result as a new token
#define JOIN4(a,b,c,d)		_JOIN4(a,b,c,d)

#define _JOIN5(a,b,c,d,e)	a##b##c##d##e
/// Join three tokens together and interpret the result as a new token
#define JOIN5(a,b,c,d,e)	_JOIN5(a,b,c,d,e)

#endif // __USEFUL_H__
