/*
    STM32 Nested Vector Interrupt Controller library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "nvic.h"

void nvic_setup (unsigned irq, unsigned priority)
{
    unsigned shift = (irq & 3) * 4;
    unsigned mask = ~(0xff << shift);
    NVIC->IP [irq / 4] = (NVIC->IP [irq / 4] & mask) | (priority << ((8 - __NVIC_PRIO_BITS) + shift));

    nvic_enable (irq);
}
