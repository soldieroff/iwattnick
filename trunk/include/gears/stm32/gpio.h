/*
    STM32 GPIO helpers library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __GPIO_H__
#define __GPIO_H__

/**
 * @file gpio.h
 *      Functions and macros to deal with GPIO ports.
 */

#include "useful.h"

/// Return the port name (A, B, C etc) given hardware feature name
#define PORT(x)			JOIN2(x, _PORT)
/// Return the bit number (0, 1, 2 etc) given hardware feature name
#define BIT(x)			JOIN2(x, _BIT)

// Auxiliary macros (not meant to be directly used by user)
#define __GPIO_PORT_MASK	0xf000
#define __GPIO_PORT_SHIFT	12
#define __GPIO_PORT_A		0x0000
#define __GPIO_PORT_B		0x1000
#define __GPIO_PORT_C		0x2000
#define __GPIO_PORT_D		0x3000
#define __GPIO_PORT_E		0x4000
#define __GPIO_PORT_F		0x5000
#define __GPIO_PORT_G		0x6000
#define __GPIO_PORT_H		0x7000
#define __GPIO_PORT_I		0x8000

#define __GPIO_BIT_MASK		0x0F00
#define __GPIO_BIT_SHIFT	8

#define __GPIO_INIST_MASK	0x00C0
#define __GPIO_INIST_X		0x0000
#define __GPIO_INIST_0		0x0040
#define __GPIO_INIST_1		0x0080

/// Preprocessor: compare the port of hw feature x with port name p
#define PORT_CMP(x, p)		(JOIN2(__GPIO_PORT_, PORT(x)) == JOIN2(__GPIO_PORT_, p))
/// Preprocessor: compare the port and bit of hw feature x with port p and bit b
#define PORTBIT_CMP(x, p, b)	(PORT_CMP(x, p) && (BIT(x) == b))
/// Preprocessor: compare the port of hw feature x with port of hw feature p
#define PORTS_CMP(x, p)		(JOIN2(__GPIO_PORT_, PORT(x)) == JOIN2(__GPIO_PORT_, PORT(p)))

/// Return the GPIO port (GPIOA, GPIOB etc) given feature name
#define GPIO(x)			JOIN2(GPIO, PORT (x))

/// Bit Value: convert bit number to bit mask
#define BV(x)			(1 << (x))
/// Return the bit value of a port bit by name
#define BITV(x)			BV(BIT(x))

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

// ----- // Simple macros to handle GPIO ports // ----- //

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
 *      GPIO (GREEN_LED) = GPIO_CONFIGURE (GPIO (GREEN_LED), GREEN_LED, OUTPUT_2MHz, PUSHPULL);
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
#define GPIO_CONFIGURE(v,b,m,c)	(((v) & ~GPIO_CNFMODE(b, MASK, MASK)) | GPIO_CNFMODE(b, m, c))

/**
 * Return a pointer to either CRL or CRH depending on the bit number of
 * the hardware feature b.
 * @arg b
 *      hw feature bit name
 */
#define GPIO_CR(b)		((BIT(b) < 8) ? &GPIO(b)->CRL : &GPIO(b)->CRH)

/// Atomic set of a single bit in port
#define GPIO_SET(x)		(GPIO(x)->BSRR = BITV (x))

/// Atomic clear of a single bit in port
#define GPIO_RESET(x)		(GPIO(x)->BRR = BITV (x))

/**
 * Define the configuration of a single GPIO port.
 * @arg p
 *      hw feature name
 * @arg m
 *      GPIO mode (without the GPIO_MODE_ prefix)
 * @arg c
 *      Port configuration (without GPIO_CNF_ prefix)
 * @arg s
 *      Initial output state (0, 1 or X)
 */
#define GPIO_CONFIG(p,m,c,s) (\
	JOIN2 (__GPIO_PORT_, PORT(p)) | \
	(BIT(p) << __GPIO_BIT_SHIFT) | \
	JOIN2 (GPIO_MODE_, m) | \
	JOIN2 (GPIO_CNF_, c) | \
	JOIN2 (__GPIO_INIST_, s) \
)

/**
 * Configure a GPIO port.
 * @arg conf
 *      GPIO port configuration (use GPIO_CONFIG() to create a bitmask)
 */
extern void gpio_config (uint16_t conf);

/**
 * Configure a number of GPIO ports at once.
 * @arg conf
 *      A pointer to an array of GPIO port configurations
 *      (use GPIO_CONFIG() to create the bitmasks)
 * @arg n
 *      Number of elements in the conf array
 */
extern void gpio_configs (const uint16_t *conf, unsigned n);

#endif // __GPIO_H__
