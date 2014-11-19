/*
    iWattnick the on-board computer for e-transport
    Public interface description
    Copyright (C) 2009-2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#ifndef __IWAIF_H__
#define __IWAIF_H__

#include <stdint.h>

/// Firmware version hi
#define IWA_VERSION_HI		0
/// Firmware version lo
#define IWA_VERSION_LO		1

/// Our address on the slave MudBus
#define IWA_BUSA_SLAVE	'@'

/**
 * iWattnick Configuration Address Space
 */
typedef struct
{

} iwa_config_t;

#endif // __IWAIF_H__
