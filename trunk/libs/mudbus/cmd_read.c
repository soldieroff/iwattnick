/*
    MudBus READ command implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
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

    uint32_t ofs = nofs & MB_OFS_MASK;

    while (ncas)
    {
        if ((ofs >= (cas->sofs & MBCAF_OFS_MASK)) &&
            (ofs < cas->eofs))
        {
            if (ofs + n > cas->eofs)
                goto e_badr;

            mb_send_hdr (mb, mb->inb [1], MBC_DATA, 2 + n);
            mb_send_nofs (mb, n, ofs);
            mb_send_last (mb, ((uint8_t *)cas->data) + ofs - (cas->sofs & MBCAF_OFS_MASK), n);
            return;
        }

        cas++;
        ncas--;
    }

e_badr:
    mb_cmd_error (mb, MBE_BADR, &nofs, sizeof (nofs));
}
