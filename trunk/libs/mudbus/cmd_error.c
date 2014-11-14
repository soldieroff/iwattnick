/*
    MudBus ERROR command implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus.h"

void mb_cmd_error (mudbus_t *mb, uint8_t err, void *data, unsigned size)
{
    DEBUG_BREAK_IF (size > 15);

    mb_send_hdr (mb, mb->inb [1], MBC_ERROR, 1 + size);
    mb_send_frag (mb, &err, sizeof (err));
    mb_send_last (mb, data, size);
}
