/*
    MudBus WRITE command implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus.h"
#include "stdfun.h"

void mb_cmd_write (mudbus_t *mb, const mb_cas_area_t *cas, unsigned ncas)
{
    uint32_t nofs = GET_NOFS (mb);
    uint32_t n = (nofs & MB_N_MASK) >> MB_N_SHIFT;
    uint8_t err;

    if (n > MB_N_MAX)
    {
        err = MBE_2BIG;
        goto error;
    }

    if (n != (uint32_t)(mb->inb [2] & MB_LEN_MASK) + 1 - 2)
    {
        err = MBE_BADR;
        goto error;
    }

    uint32_t ofs = nofs & MB_OFS_MASK;

    while (ncas)
    {
        if ((ofs >= (cas->sofs & MBCAF_OFS_MASK)) &&
            (ofs < cas->eofs))
        {
            if (ofs + n > cas->eofs)
            {
                err = MBE_BADR;
                goto error;
            }

            if (cas->sofs & MBCAF_WP)
            {
                err = MBE_ACCESS;
                goto error;
            }

            memcpy (((uint8_t *)cas->data) + ofs - (cas->sofs & MBCAF_OFS_MASK),
                mb->inb + MB_HDR_LEN + 2, n);

            err = MBE_NONE;
            goto error;
        }

        cas++;
        ncas--;
    }

    err = MBE_BADR;

error:
    mb_cmd_error (mb, err, &nofs, sizeof (nofs));
}
