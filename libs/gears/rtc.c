/*
    Real-Time-Clock library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "rtc.h"

void rtc_done_write ()
{
    // Get rid of the 'volatile' attribute, gcc generates suboptimal code
    *(uint16_t *)&RTC->CRL &= ~RTC_CRL_CNF;
}

void rtc_init ()
{
    // Включим питание RTC
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    // Разрешим доступ на запись в регистры RTC
    PWR->CR |= PWR_CR_DBP;

    // Запускаем процесс обновления образов регистров RTC
    rtc_invalidate ();
    // Ждём синхронизации регистров RTC с их бледными копиями на APB1
    rtc_wait_sync ();

    // Если часы не в нужном режиме работы, делаем полный сброс и конфигурируем заново
    if ((RCC->BDCR & (RCC_BDCR_LSEON | RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL)) ==
        (RCC_BDCR_LSEON | RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE))
        return;

    // Полностью сбросим часы и энергонезависимое хранилище
    RCC->BDCR = RCC_BDCR_BDRST;
    // Включим внешний кварц
    RCC->BDCR = RCC_BDCR_LSEON;
    // Ждём пока кварц заведётся (можно не ждать, но для порядка...)
    while (!(RCC->BDCR & RCC_BDCR_LSERDY)) ;
    // Запустим тактирование RTC от внешнего кварца
    RCC->BDCR = RCC_BDCR_LSEON | RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;

    // Начальная настройка RTC
    rtc_write ()
    {
        // We want 16 ticks per second
        RTC->PRLH = (uint16_t)(((RTC_FREQ / RTC_TICKS_PER_SEC) - 1) >> 16);
        RTC->PRLL = (uint16_t)((RTC_FREQ / RTC_TICKS_PER_SEC) - 1);
        rtc_set_counter (0);
        rtc_set_alarm (0xffffffff);
    }
}
