/*
    MudBus protocol implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "gears.h"
#include "mudbus.h"

#define mbm_dma_copy		JOIN3 (dma, DMA_NUM (MBM_USART), _copy)

mudbus_master_t mbm;

uint8_t mb_crc8_update (uint8_t crc8, uint8_t c)
{
    crc8 ^= c;

    for (uint8_t i = 0; i < 8; i++)
        crc8 = (crc8 & 0x80) ? ((crc8 << 1) ^ MB_CRC8_POLY) : (crc8 << 1);

    return crc8;
}

uint8_t mb_crc8 (uint8_t *data, uint8_t len)
{
    uint8_t crc8 = MB_CRC8_INIT;
    while (len--)
        crc8 = mb_crc8_update (crc8, *data++);
    return crc8;
}

void mbm_init ()
{
    memset (&mbm, 0, sizeof (mbm));

    mbm.flags = MBMF_EMPTYQ;
}

/// Kick DMA to send next fragment
static void mbm_send_next ()
{
    const uint8_t *data;
    uint8_t len;

    // If transmitter is still busy, will do it later
    if (!(USART (MBM)->SR & USART_SR_TXE))
        return;

    if (mbm.flags & MBMF_EMPTYQ)
    {
        if (!(mbm.flags & MBMF_CRC8))
        {
            // will send CRC8
            mbm.flags |= MBMF_CRC8;
            USART (MBM)->DR = mbm.crc8;
            return;
        }
        else if (!(mbm.flags & MBMF_SPACE))
        {
            // will send a 4-char space
            mbm.flags |= MBMF_SPACE;

            // mute TX
            mbm_usart_tx_mute (true);

            // Send any 4 bytes
            data = (uint8_t *)&mbm;
            len = 4;
        }
    }
    else
    {
        data = mbm.queue_data [mbm.queue_tail];
        len = mbm.queue_len [mbm.queue_tail];

        // Advance queue tail
        mbm.queue_tail = (mbm.queue_tail + 1) & (MBM_QUEUE_SIZE - 1);

        if (mbm.queue_tail == mbm.queue_head)
            mbm.flags |= MBMF_EMPTYQ;
    }

    (void)data;
    (void)len;
}

bool mbm_send (const uint8_t *data, uint8_t len)
{
    // If this is the first fragment, initialize CRC8
    if (mbm.flags & MBMF_EMPTYQ)
        mbm.crc8 = MB_CRC8_INIT;
    else if (mbm.queue_head == mbm.queue_tail)
        return false; // no space in queue

    // Store the fragment in the queue
    mbm.queue_data [mbm.queue_head] = data;
    mbm.queue_len [mbm.queue_head] = len;

    // Advance queue head
    mbm.flags &= ~MBMF_EMPTYQ;
    mbm.queue_head = (mbm.queue_head + 1) & (MBM_QUEUE_SIZE - 1);

    // update CRC8
    while (len--)
        mbm.crc8 = mb_crc8_update (mbm.crc8, *data++);

    mbm_send_next ();

    return true;
}
