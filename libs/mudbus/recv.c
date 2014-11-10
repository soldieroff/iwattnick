/*
    MudBus data reception
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gears.h"
#include "mudbus.h"

void mb_recv_next (mudbus_t *mb)
{
    if (mbd_rx_busy (&mb->driver) || (mb->flags & MBX_RX_DIGEST))
        return;

    uint8_t len;

    if (mb->inb_len == 0)
        // Set up to receive packet header
        len = 3;
    else if (mb->inb_len == 3)
    {
        if ((mb->inb [0] != MB_BUSA_BROADCAST) &&
            (mb->inb [0] != mb->addr))
        {
            // Break the UART -> DMA link, will reset on IDLE
            mbd_rx_stop (&mb->driver);
            return;
        }

        // Got header, now get packet body and CRC8
        len = (mb->inb [1] & MB_LEN_MASK) + 1 + 1;
    }
    else
    {
        // Got the whole packet, check the CRC8
        len = 3 + (mb->inb [1] & MB_LEN_MASK) + 1 + 1;
        if (mb_crc8 (mb->inb, len) == 0)
        {
            mb->flags |= MBX_RX_DIGEST;
            mb_user_recv (mb);
            mb->flags &= ~MBX_RX_DIGEST;
        }

        // Discard the received packet, listen for the next header
        mb->inb_len = 0;
        len = 3;
    }

    mbd_rx_start (&mb->driver, mb->inb + mb->inb_len, len);
    mb->inb_len += len;
}

void mb_recv_error (mudbus_t *mb)
{
    mbd_rx_stop (&mb->driver);
    mb->inb_len = 0;
    mb_recv_next (mb);
}
