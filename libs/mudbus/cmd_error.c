/*
    MudBus READ command implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gears.h"
#include "mudbus.h"

void mb_cmd_error (mudbus_t *mb, uint8_t err, void *data, unsigned size)
{
    (void)mb;
    (void)err;
    (void)data;
    (void)size;
}
