/*
    Basic macros
    Copyright (C) 2009 by Andrew Zabolotny <zap@cobra.ru>
*/

#ifndef __MACROS_H__
#define __MACROS_H__

#define _JOIN2(a, b)		a##b
#define JOIN2(a, b)		_JOIN2(a, b)
#define _JOIN3(a,b,c)		a##b##c
#define JOIN3(a, b, c)		_JOIN3(a,b,c)

#define ARRAY_LEN(x)		(sizeof (x) / sizeof (x [0]))

#endif /* __MACROS_H__ */
