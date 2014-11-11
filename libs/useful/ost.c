/*
    One-shot timers
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include "ost.h"

void ost_arm (uint32_t *timer, uint32_t clocks)
{
    uint32_t t = clock + clocks;
    if (!t) t = 1;
    *timer = t;
}

bool ost_expired (uint32_t *timer)
{
    if (!ost_enabled (*timer))
        return true;

    if (((int32_t)(*timer - clock)) > 0)
        return false;

    ost_disable (timer);
    return true;
}
