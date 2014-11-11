/*
    MudBus protocol implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus.h"
#include "gears.h"

void mb_init (mudbus_t *mb, uint8_t addr)
{
    mb->addr = addr;

    memclr (&mb->flags, OFFSETOF (mudbus_t, addr) - OFFSETOF (mudbus_t, flags));

    // Set up UART & DMA to receive packets
    mb_recv_next (mb);
}
