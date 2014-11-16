/*
    iWattnick the on-board computer for e-transport
    Copyright (C) 2009-2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#ifndef __IWATTNICK_H__
#define __IWATTNICK_H__

#include HARDWARE_H
#include "iwaif.h"
#include "gears.h"

extern iwa_config_t iwa_config;

extern void hardware_init ();
extern void mudbus_init ();

#endif // __IWATTNICK_H__
