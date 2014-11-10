/*
    STM32 Nested Vector Interrupt Controller library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __NVIC_H__
#define __NVIC_H__

/**
 * @file nvic.h
 *      A set of routines to manipulate the Nested Vector
 *      Interrupt Controller (NVIC).
 */

/**
 * Set up the interrupt controller to handle the given IRQ.
 * Note that this does not include first 16 faults, which have
 * a negative index. The faults work differently: they are
 * not affected by __enable/disable_irq() and their priorities
 * is set via System Control Block (SCB), not NVIC.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 * @arg priority
 *      IRQ priority (0-15). Note that the actual interpretation
 *      of the priority value depends on the setting of the
 *      PRIGROUP bits in SCB->AIRCR.
 */
extern void nvic_setup (unsigned irq, unsigned priority);

/**
 * Just enable the given IRQ. Also don't forget to __enable_irq()
 * to allow interrupt processing in general.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 */
static inline void nvic_enable (unsigned irq)
{ NVIC->ISER [irq / 32] |= 1 << (irq & 31); }

/**
 * Disable the given IRQ.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 */
static inline void nvic_disable (unsigned irq)
{ NVIC->ICER [irq / 32] |= 1 << (irq & 31); }

/**
 * Set the pending flag on given IRQ.
 * The processor will invoke the respective IRQ handler
 * as soon as possible, if IRQ is enabled.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 */
static inline void nvic_set_pending (unsigned irq)
{ NVIC->ISPR [irq / 32] |= 1 << (irq & 31); }

/**
 * Clear the pending flag on given IRQ.
 * This is useful for edge-triggered interrupts; to acknowledge
 * level-triggered interrupts you must clear the interrupt pending
 * status directly in the hardware connected to this IRQ.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 */
static inline void nvic_clear_pending (unsigned irq)
{ NVIC->ICPR [irq / 32] |= 1 << (irq & 31); }

/**
 * Check if given IRQ is active.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 */
static inline unsigned nvic_is_active (unsigned irq)
{ return NVIC->IABR [irq / 32] & (1 << (irq & 31)); }

/**
 * Software trigger of given IRQ.
 * @arg irq
 *      IRQ number (0-67, one of the >=0 xxx_IRQn constants).
 */
static inline void nvic_irq_trigger (unsigned irq)
{ NVIC->STIR = irq; }

#endif // __NVIC_H__
