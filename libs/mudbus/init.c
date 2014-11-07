/*
    MudBus protocol implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus.h"
#include "gears.h"

void mb_init (mudbus_t *mb)
{
    // Fill some fields with zeros
    memclr (mb + OFFSETOF (mudbus_t, queue_head),
        OFFSETOF (mudbus_t, inb) - OFFSETOF (mudbus_t, queue_head));

    mb->flags = MBF_EMPTYQ;
}
