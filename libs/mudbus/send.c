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

    // If last fragment in queue was sent, advance queue tail
    if (mb->flags & MBF_TX_Q)
    {
        mb->flags &= ~MBF_TX_Q;

        // Advance queue tail
        mb->queue_tail = (mb->queue_tail + 1) & (MBM_QUEUE_SIZE - 1);
        if (mb->queue_tail == mb->queue_head)
            mb->flags |= MBF_EMPTYQ;
    }

    if (mb->flags & MBF_EMPTYQ)
    {
        data = (uint8_t *)&mb->crc8;

        if (!(mb->flags & MBF_TX_CRC8))
        {
            // will send CRC8
            mb->flags |= MBF_TX_CRC8;
            len = 1;
        }
        else if (!(mb->flags & MBF_TX_SPACE))
        {
            // will send a 4-char space
            mb->flags |= MBF_TX_SPACE;

            // Send any byte (TX will be muted)
            len = 1;
        }
        else
        {
            // If the space transmission is not complete, wait USART IRQ
            if (!mbd_tx_complete (&mb->driver))
                return;

            // all done
            mb->flags &= ~(MBF_TX_CRC8 | MBF_TX_SPACE);
            // Don't send anything
            len = 0;
        }
    }
    else
    {
        data = mb->queue_data [mb->queue_tail];
        len = mb->queue_len [mb->queue_tail];

        mb->flags = (mb->flags & ~(MBF_TX_CRC8 | MBF_TX_SPACE)) | MBF_TX_Q;
    }

    // While sending space, mute TX, enable IRQ on TC
    mbd_tx_mute (&mb->driver, mb->flags & MBF_TX_SPACE);

    // Send the data
    if (len)
        mbd_tx_start (&mb->driver, data, len);
}

void mb_send_stop (mudbus_t *mb)
{
    mbd_tx_stop (&mb->driver);
    mb->queue_tail = mb->queue_head;
    mb->flags = (mb->flags & ~(MBF_TX_CRC8 | MBF_TX_SPACE | MBF_TX_Q)) | MBF_EMPTYQ;
}

void mb_send (mudbus_t *mb, const uint8_t *data, uint8_t len)
{
    // If this is the first fragment, initialize CRC8
    if (mb->flags & MBF_EMPTYQ)
        mb->crc8 = MB_CRC8_INIT;
    else if (mb->queue_head == mb->queue_tail)
        return; // no space in queue

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
}
