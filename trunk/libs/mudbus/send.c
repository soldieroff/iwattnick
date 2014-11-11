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
    // If transmitter is still busy, will do it later
    if (mbd_tx_busy (&mb->driver))
        return;

    // If we have finished, stop sending
    if (mb->flags & MBF_TX_SPACE)
    {
        // all done
        mb->flags &= ~MBF_TX_SPACE;

        // Disable USART TC interrupt, will re-enable when required
        mbd_tx_irq (&mb->driver, false);

        return;
    }

    uint8_t len;

    // After we have sent the packet itself, send the space
    if (mb->flags & MBF_TX_OUTB)
    {
        mb->outb_len = 0;
        mb->flags = (mb->flags & ~MBF_TX_OUTB) | MBF_TX_SPACE;

        // Send any byte (TX will be muted)
        len = 1;
    }
    else
    {
        mb->flags |= MBF_TX_OUTB;
        len = mb->outb_len;
    }

    // While sending space, mute TX, enable IRQ on TC
    mbd_tx_mute (&mb->driver, mb->flags & MBF_TX_SPACE);

    // Enable transmission complete IRQ
    mbd_tx_irq (&mb->driver, true);

    // Send the data
    mbd_tx_start (&mb->driver, mb->outb, len);
}

void mb_send_stop (mudbus_t *mb)
{
    mbd_tx_stop (&mb->driver);
    mb->outb_len = 0;
    mb->flags &= ~MBF_TX_SPACE;
}

void mb_send_frag (mudbus_t *mb, const uint8_t *data, uint8_t len)
{
    if (mb->outb_len + len > sizeof (mb->outb) - 1)
        return;

    memcpy (&mb->outb + mb->outb_len, data, len);
    mb->outb_len += len;
}

void mb_send_last (mudbus_t *mb, const uint8_t *data, uint8_t len)
{
    mb_send_frag (mb, data, len);

    // Append packet CRC8
    mb->outb [mb->outb_len] = mb_crc8 (mb->outb, mb->outb_len);
    mb->outb_len++;

    // Go!
    mb_send_next (mb);
}
