/*
    iWattnick the on-board computer for e-transport
    Copyright (C) 2009-2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#include "iwattnick.h"

int main (void)
{
    hardware_init ();

    for (;;)
    {

        // До следующего прерывания нам делать абсолютно нечего
        //__WFI ();
    }
}
