/*
    Basic macros
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __MACROS_H__
#define __MACROS_H__

/**
 * @file macros.h
 *      The basic corpus of preprocessor macros. From generic macros
 *      of general use to hardware-specialized ones. Normally you don't
 *      include it directly as it's included from HARDWARE_H.
 */

#include <stdint.h>

// ----- // Generic macros // ----- //

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

// ----- // Hardware-related macros // ----- //

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

/// Return the port name (A, B, C etc) given hardware feature name
#define PORT(x)			JOIN2(x, _PORT)
/// Return the bit number (0, 1, 2 etc) given hardware feature name
#define BIT(x)			JOIN2(x, _BIT)
/// Return the IRQ priority corresponding to this hardware feature
#define IRQ_PRIO(x)		JOIN2(x, _IRQ_PRIO)
/// Return the DMA controller number (1, 2, ...) given hardware feature name
#define DMA_NUM(x)		JOIN2(x, _DMA_NUM)
/// Return the DMA channel number (1-7) given hardware feature name
#define DMA_CHAN(x)		JOIN2(x, _DMA_CHAN)
/// Return the IRQ number corresponding to this DMA channel
#define DMA_IRQ(x)		JOIN5(DMA, DMA_NUM(x), _Channel, DMA_CHAN (x), _IRQn)
/// Return the DMA IRQ priority corresponding to this hardware feature
#define DMA_IRQ_PRIO(x)		JOIN2(x, _DMA_IRQ_PRIO)
/// Return the USART instance corresponding to a hardware feature (e.g. USART(DEBUG) -> USART3)
#define USART(x)		JOIN2 (USART, JOIN2(x, _USART))

/// Return the GPIO port (GPIOA, GPIOB etc) given feature name
#define GPIO(x)			JOIN2(GPIO, PORT (x))

/// Bit Value: convert bit number to bit mask
#define BV(x)			(1 << (x))
/// Return the bit value of a port bit by name
#define BITV(x)			BV(BIT(x))

#define __CMP_PORTA		99990
#define __CMP_PORTB		99991
#define __CMP_PORTC		99992
#define __CMP_PORTD		99993
#define __CMP_PORTE		99994
#define __CMP_PORTF		99995
#define __CMP_PORTG		99996

#define __CMP_USART1		99980
#define __CMP_USART2		99981
#define __CMP_USART3		99982
#define __CMP_USART4		99983
#define __CMP_USART5		99984

/// Preprocessor: compare the port of hw feature x with port name p
#define PORT_CMP(x, p)		(JOIN2(__CMP_PORT, PORT(x)) == JOIN2(__CMP_PORT, p))
/// Preprocessor: compare the port and bit of hw feature x with port p and bit b
#define PORTBIT_CMP(x, p, b)	(PORT_CMP(x, p) && (BIT(x) == b))
/// Preprocessor: compare the port of hw feature x with port of hw feature p
#define PORTS_CMP(x, p)		(JOIN2(__CMP_PORT, PORT(x)) == JOIN2(__CMP_PORT, PORT(p)))
/// Check if the USART of a hw feature is same as expected (p = 1, 2 etc)
#define USART_CMP(x,p)		(JOIN2 (__CMP_USART, JOIN2 (x, _USART)) == JOIN2(__CMP_USART, p))

// ----- // GPIO configuration bits (undefined for some reason by stmxxxx.h) // ----- //

/// Input mode (reset state)
#define GPIO_MODE_INPUT		0
/// Output mode, max speed 10 MHz
#define GPIO_MODE_OUTPUT_10MHz	1
/// Output mode, max speed 2 MHz
#define GPIO_MODE_OUTPUT_2MHz	2
/// Output mode, max speed 50 MHz
#define GPIO_MODE_OUTPUT_50MHz	3
/// Just the bitmask for all bits
#define GPIO_MODE_MASK		3

// In input mode (MODE[1:0]=00)

/// Analog mode
#define GPIO_CNF_ANALOG		0
/// Floating input (reset state)
#define GPIO_CNF_FLOATING	4
/// Input with pull-up / pull-down
#define GPIO_CNF_PUD		8

// In output mode (MODE[1:0] > 00)

/// General purpose output push-pull
#define GPIO_CNF_PUSHPULL	0
/// General purpose output Open-drain
#define GPIO_CNF_OPENDRAIN	4
/// Alternate function output Push-pull
#define GPIO_CNF_AF_PUSHPULL	8
/// Alternate function output Open-drain
#define GPIO_CNF_AF_OPENDRAIN	12

/// Just the bitmask for all bits
#define GPIO_CNF_MASK		12

/**
 * Return the bits for mode m & config c for GPIO bit b.
 * Example:
 * @code
 *      bits = GPIO_CNFMODE (GREEN_LED, OUTPUT_2MHz, PUSHPULL);
 * @endcode
 * @arg b
 *      hw feature bit name
 * @arg m
 *      Port mode (without GPIO_MODE_ prefix)
 * @arg c
 *      Port configuration (without GPIO_CNF_ prefix)
 */
#define GPIO_CNFMODE(b,m,c)	(JOIN2 (GPIO_CNF_, c) << ((BIT(b) & 7) * 4) | \
				 JOIN2 (GPIO_MODE_, m) << ((BIT(b) & 7) * 4))

/**
 * Drop given bits in old GPIO CRL/CRH and set according to new mode & config.
 * Example:
 * @code
 *      GPIO (GREEN_LED) = GPIO_SET (GPIO (GREEN_LED), GREEN_LED, OUTPUT_2MHz, PUSHPULL);
 * @endcode
 * @arg v
 *      The old value of the configuration register (or a constant)
 * @arg b
 *      hw feature bit name
 * @arg m
 *      Mode bits (without GPIO_MODE_ prefix)
 * @arg c
 *      Configuration bits (without GPIO_CNF_ prefix)
 */
#define GPIO_SET(v,b,m,c)	(((v) & ~GPIO_CNFMODE(b, MASK, MASK)) | GPIO_CNFMODE(b, m, c))

/**
 * Return a pointer to either CRL or CRH depending on the bit number of
 * the hardware feature b.
 * @arg b
 *      hw feature bit name
 */
#define GPIO_CR(b)		((BIT(b) < 8) ? &GPIO(b)->CRL : &GPIO(b)->CRH)

/// Atomic set of a single bit in port
#define GPIO_BSET(x)		(GPIO(x)->BSRR |= BITV (x))

/// Atomic clear of a single bit in port
#define GPIO_BRESET(x)		(GPIO(x)->BRR |= BITV (x))

#endif /* __MACROS_H__ */
