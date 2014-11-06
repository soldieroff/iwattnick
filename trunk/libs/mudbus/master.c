/*
    MudBus protocol implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gears.h"
#include "mudbus.h"

#if 0

// Substantialy slower but 24 bytes smaller
static inline uint8_t mb_crc8_update (uint8_t crc8, uint8_t c)
{
    crc8 ^= c;

    for (uint8_t i = 0; i < 8; i++)
        crc8 = (crc8 & 0x80) ? ((crc8 << 1) ^ MB_CRC8_POLY) : (crc8 << 1);

    return crc8;
}

#else

static const uint8_t crc8tab [16] =
{
    0x00, 0x31, 0x62, 0x53, 0xc4, 0xf5, 0xa6, 0x97,
    0xb9, 0x88, 0xdb, 0xea, 0x7d, 0x4c, 0x1f, 0x2e
};

static inline uint8_t mb_crc8_update (uint8_t crc8, uint8_t c)
{
    crc8 ^= c;

    crc8 = (crc8 << 4) ^ crc8tab [(crc8 >> 4) & 15];
    crc8 = (crc8 << 4) ^ crc8tab [(crc8 >> 4) & 15];

    return crc8;
}

#endif

uint8_t mb_crc8 (uint8_t *data, int len)
{
    uint8_t crc8 = MB_CRC8_INIT;
    while (len--)
        crc8 = mb_crc8_update (crc8, *data++);
    return crc8;
}

void mbm_init (mudbus_t *mb)
{
    memset (mb, 0, sizeof (mudbus_t));

    mb->flags = MBMF_EMPTYQ;
    mbd_master_init (&mb->driver);
}

/// Kick DMA to send next fragment
void mb_send_next (mudbus_t *mb)
{
    const uint8_t *data;
    uint8_t len;

    // If transmitter is still busy, will do it later
    if (mbd_tx_busy (&mb->driver))
        return;

    if (mb->flags & MBMF_TAILSENT)
    {
        // Advance queue tail
        mb->queue_tail = (mb->queue_tail + 1) & (MBM_QUEUE_SIZE - 1);
        if (mb->queue_tail == mb->queue_head)
            mb->flags |= MBMF_EMPTYQ;
        mb->flags &= ~MBMF_TAILSENT;
    }

    if (mb->flags & MBMF_EMPTYQ)
    {
        if (!(mb->flags & MBMF_CRC8))
        {
            // will send CRC8
            mb->flags |= MBMF_CRC8;
            data = (uint8_t *)&mb->crc8;
            len = 1;
        }
        else if (!(mb->flags & MBMF_SPACE))
        {
            // will send a 4-char space
            mb->flags |= MBMF_SPACE;

            // mute TX
            mbd_tx_mute (&mb->driver, true);

            // Send any 4 bytes
            data = (uint8_t *)mb;
            len = 4;
        }
        else
        {
            // all done
            mb->flags &= ~(MBMF_CRC8 | MBMF_SPACE);
            return;
        }
    }
    else
    {
        mbd_tx_mute (&mb->driver, false);

        data = mb->queue_data [mb->queue_tail];
        len = mb->queue_len [mb->queue_tail];

        mb->flags |= MBMF_TAILSENT;
    }

    mbd_tx_start (&mb->driver, data, len);
}

void mb_send_stop (mudbus_t *mb)
{
    mbd_tx_stop (&mb->driver);
    mb->queue_tail = mb->queue_head;
    mb->flags = MBMF_EMPTYQ;
}

bool mb_send (mudbus_t *mb, const uint8_t *data, uint8_t len)
{
    // If still sending CRC8 and space of previous packet, we're busy
    if (mb->flags & (MBMF_CRC8 | MBMF_SPACE))
        return false;

    // If this is the first fragment, initialize CRC8
    if (mb->flags & MBMF_EMPTYQ)
        mb->crc8 = MB_CRC8_INIT;
    else if (mb->queue_head == mb->queue_tail)
        return false; // no space in queue

    // Store the fragment in the queue
    mb->queue_data [mb->queue_head] = data;
    mb->queue_len [mb->queue_head] = len;

    // Advance queue head
    mb->flags &= ~MBMF_EMPTYQ;
    mb->queue_head = (mb->queue_head + 1) & (MBM_QUEUE_SIZE - 1);

    // update CRC8
    while (len--)
        mb->crc8 = mb_crc8_update (mb->crc8, *data++);

    mb_send_next (mb);

    return true;
}
