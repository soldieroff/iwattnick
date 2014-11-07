/*
    STM32 Real-Time-Clock library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __OST_H__
#define __OST_H__

/**
 * @file ost.h
 *      Simple one-shot timers.
 *      Use the global 'clock' variable which is supposed to be incremented
 *      some times per second.
 */

/**
 * The system clock variable.
 */
extern volatile uint32_t clock;

// CLOCKS_PER_SEC is expected to be defined in hardware.h
#ifndef CLOCKS_PER_SEC
#  define CLOCKS_PER_SEC	32
#endif

/** Seconds to clocks */
#define CLOCKS(s)		((uint32_t)((s) * CLOCKS_PER_SEC))

/**
 * Взвести одноразовый таймер (one-shot timer).
 * Таймер "сработает" через заданное число щелчков системного таймера.
 * @arg timer
 *      Указатель на переменную-таймер
 * @arg clocks
 *      Число щелчков, через которые должен сработать таймер
 */
extern void ost_arm (uint32_t *timer, uint32_t clocks);

/**
 * Проверить, не истёк ли одноразовый таймер,
 * также возвращает не ноль если таймер отключён.
 * @arg timer
 *      Указатель на переменную-таймер
 * @return
 *      true если таймер истёк либо выключен
 */
extern bool ost_expired (uint32_t *timer);

/**
 * Проверить, включён ли таймер
 */
#define ost_enabled(timer) \
    (timer != 0)

/**
 * Выключить таймер
 */
#define ost_disable(timer) \
    *(timer) = 0

#endif // __RTC_H__
