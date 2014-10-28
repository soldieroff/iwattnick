/*
    Real-Time-Clock library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __RTC_H__
#define __RTC_H__

/// How many RTC ticks per second we want (1 = 1 tick per second)
#define RTC_TICKS_PER_SEC	16

/// Call after APB1 reset or APB1 clock stop (including reset)
#define rtc_invalidate() \
    *(uint16_t *)&RTC->CRL &= ~RTC_CRL_RSF

/// Call after APB1 reset or APB1 clock stop (including reset) to ensure RTC regs valid
#define rtc_wait_sync() \
    while (!(RTC->CRL & RTC_CRL_RSF)) \
        ;

/**
 * Wait until a write to RTC registers is possible, and enable writing.
 * Use rtc_wait_write()/rtc_done_write() to mark the start and the end
 * of a RTC registers write block.
 */
static __inline__ void rtc_wait_write ()
{
    while (!(RTC->CRL & RTC_CRL_RTOFF))
        ;
    *(uint16_t *)&RTC->CRL |= RTC_CRL_CNF;
}

/// Disable writing to RTC registers
static __inline__ void rtc_done_write ()
{
    // Get rid of the 'volatile' attribute, gcc generates suboptimal code
    *(uint16_t *)&RTC->CRL &= ~RTC_CRL_CNF;
}

/// Get the current RTC seconds counter
static __inline__ uint32_t rtc_counter ()
{
    return  ((uint32_t)RTC->CNTL) | (((uint32_t)RTC->CNTH) << 16);
}

/// Set the RTC seconds counter
static __inline__ void rtc_set_counter (uint32_t x)
{
    RTC->CNTL = (uint16_t)x;
    RTC->CNTH = x >> 16;
}

/// Get the current RTC alarm counter
static __inline__ uint32_t rtc_alarm ()
{
    return ((uint32_t)RTC->ALRL) | (((uint32_t)RTC->ALRH) << 16);
}

/// Set the RTC alarm counter
static __inline__ void rtc_set_alarm (uint32_t x)
{
    RTC->ALRL = (uint16_t)x;
    RTC->ALRH = x >> 16;
}

/**
 * Initialize the RTC.
 * Checks the RTC and if it has not been initialized yet,
 * sets it to emit RTC_TICKS_PER_SEC ticks per second.
 */
extern void rtc_init ();

#endif // __RTC_H__
