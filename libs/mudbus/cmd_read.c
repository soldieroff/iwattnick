/*
    MudBus READ command implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gears.h"
#include "mudbus.h"

void mb_cmd_read (mudbus_t *mb, const mb_cas_area_t *cas, unsigned ncas)
{
    uint32_t nofs = GET_NOFS (mb);
    uint32_t n = (nofs & MB_N_MASK) >> MB_N_SHIFT;

    if (n > MB_N_MAX)
    {
        mb_cmd_error (mb, MBE_2BIG, &nofs, sizeof (nofs));
        return;
    }

    nofs &= ~MB_N_MASK;

    while (ncas)
    {
        if ((nofs >= cas->sofs) &&
            (nofs < cas->eofs))
        {
            //mb_send_hdr (mb->inb [1], MBC_DATA, 2 + n);
            return;
        }

        cas++;
        ncas--;
    }
}
