/*
    Basic macros
    Copyright (C) 2009 by Andrew Zabolotny <zap@cobra.ru>
*/

#ifndef __MACROS_H__
#define __MACROS_H__

#define _JOIN2(a, b)		a##b
/// Join two tokens together and interpret the result as a new token
#define JOIN2(a, b)		_JOIN2(a, b)
#define _JOIN3(a,b,c)		a##b##c
/// Join three tokens together and interpret the result as a new token
#define JOIN3(a, b, c)		_JOIN3(a,b,c)

/// Get the number of elements in a static array
#define ARRAY_LEN(x)		(sizeof (x) / sizeof (x [0]))

/// Return the port name (A, B, C etc) given hardware feature name
#define PORT(x)			JOIN2(x, _PORT)
/// Return the bit number (0, 1, 2 etc) given hardware feature name
#define BIT(x)			JOIN2(x, _BIT)

/// Return the GPIO port (GPIOA, GPIOB etc) given feature name
#define GPIO(x)			JOIN2(GPIO, PORT (x))

/// Bit Value: convert bit number to bit mask
#define BV(x)			(1 << (x))
/// Return the bit value of a port bit by name
#define BITV(x)			BV(BIT(x))

#define __PORT_CMP_A		99990
#define __PORT_CMP_B		99991
#define __PORT_CMP_C		99992
#define __PORT_CMP_D		99993
#define __PORT_CMP_E		99994
#define __PORT_CMP_F		99995
#define __PORT_CMP_G		99996

/// Preprocessor: compare the port of hw feature x with port name p
#define PORT_CMP(x, p)		(JOIN2(__PORT_CMP_, PORT(x)) == JOIN2(__PORT_CMP_, p))
/// Preprocessor: compare the port of hw feature x with port of hw feature p
#define PORTS_CMP(x, p)		(JOIN2(__PORT_CMP_, PORT(x)) == JOIN2(__PORT_CMP_, PORT(p)))
/// Preprocessor: compare the port and bit of hw feature x with port p and bit b
#define PORTBIT_CMP(x, p, b)	(PORT_CMP(x, p) && (BIT(x) == b))

/// Atomic set of a single bit in port
#define BSET(x)			(GPIO(x)->BSRR |= BITV (x))
/// Atomic clear of a single bit in port
#define BRESET(x)		(GPIO(x)->BRR |= BITV (x))

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
#define GPIO_CNF_FLOATING	1
/// Input with pull-up / pull-down
#define GPIO_CNF_PUD		2

// In output mode (MODE[1:0] > 00)

/// General purpose output push-pull
#define GPIO_CNF_PUSHPULL	0
/// General purpose output Open-drain
#define GPIO_CNF_OPENDRAIN	1
/// Alternate function output Push-pull
#define GPIO_CNF_AF_PUSHPULL	2
/// Alternate function output Open-drain
#define GPIO_CNF_AF_OPENDRAIN	3

/// Just the bitmask for all bits
#define GPIO_CNF_MASK		3

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
#define GPIO_CNFMODE(b,m,c)	(JOIN2 (GPIO_CNF_, c) << ((BIT(b) & 7) * 4 + 2) | \
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

#endif /* __MACROS_H__ */
