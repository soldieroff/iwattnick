/*
    MudBus data transmission
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gears.h"
#include "mudbus.h"

/// Kick DMA to send next fragment
void mb_send_next (mudbus_t *mb)
{
    const uint8_t *data;
    uint8_t len;

    // If transmitter is still busy, will do it later
    if (mbd_tx_busy (&mb->driver))
        return;

    if (mb->flags & MBF_TAILSENT)
    {
        // Advance queue tail
        mb->queue_tail = (mb->queue_tail + 1) & (MBM_QUEUE_SIZE - 1);
        if (mb->queue_tail == mb->queue_head)
            mb->flags |= MBF_EMPTYQ;
        mb->flags &= ~MBF_TAILSENT;
    }

    if (mb->flags & MBF_EMPTYQ)
    {
        if (!(mb->flags & MBF_CRC8))
        {
            // will send CRC8
            mb->flags |= MBF_CRC8;
            data = (uint8_t *)&mb->crc8;
            len = 1;
        }
        else if (!(mb->flags & MBF_SPACE))
        {
            // will send a 4-char space
            mb->flags |= MBF_SPACE;

            // mute TX
            mbd_tx_mute (&mb->driver, true);

            // Send any 4 bytes
            data = (uint8_t *)mb;
            len = 4;
        }
        else
        {
            // all done
            mb->flags &= ~(MBF_CRC8 | MBF_SPACE);
            return;
        }
    }
    else
    {
        mbd_tx_mute (&mb->driver, false);

        data = mb->queue_data [mb->queue_tail];
        len = mb->queue_len [mb->queue_tail];

        mb->flags |= MBF_TAILSENT;
    }

    mbd_tx_start (&mb->driver, data, len);
}

void mb_send_stop (mudbus_t *mb)
{
    mbd_tx_stop (&mb->driver);
    mb->queue_tail = mb->queue_head;
    mb->flags = MBF_EMPTYQ;
}

bool mb_send (mudbus_t *mb, const uint8_t *data, uint8_t len)
{
    // If still sending CRC8 and space of previous packet, we're busy
    if (mb->flags & (MBF_CRC8 | MBF_SPACE))
        return false;

    // If this is the first fragment, initialize CRC8
    if (mb->flags & MBF_EMPTYQ)
        mb->crc8 = MB_CRC8_INIT;
    else if (mb->queue_head == mb->queue_tail)
        return false; // no space in queue

    // Store the fragment in the queue
    mb->queue_data [mb->queue_head] = data;
    mb->queue_len [mb->queue_head] = len;

    // Advance queue head
    mb->flags &= ~MBF_EMPTYQ;
    mb->queue_head = (mb->queue_head + 1) & (MBM_QUEUE_SIZE - 1);

    // update CRC8
    while (len--)
        mb->crc8 = mb_crc8_update (mb->crc8, *data++);

    mb_send_next (mb);

    return true;
}
