/*
    MudBus data transmission
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus.h"
#include "usefun.h"

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

    // Send the data
    mbd_tx_start (&mb->driver, mb->outb, len);

    // Enable transmission complete IRQ
    mbd_tx_irq (&mb->driver, true);
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

    memcpy (mb->outb + mb->outb_len, data, len);
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

void mb_send_hdr (mudbus_t *mb, uint8_t busa, uint8_t cmd, uint8_t len)
{
    DEBUG_BREAK_IF (!mb_can_send (mb));
    DEBUG_BREAK_IF (cmd & ~MB_CMD_MASK);
    DEBUG_BREAK_IF ((len == 0) || (len > 16));

    // Create the packet header directly in mb->outb
    mb->outb [0] = busa;
    mb->outb [1] = mb->busa;
    mb->outb [2] = cmd | (len - 1);
    mb->outb_len = 3;
}

void mb_send_nofs (mudbus_t *mb, uint8_t n, uint32_t ofs)
{
    DEBUG_BREAK_IF (n > MB_N_MAX);
    DEBUG_BREAK_IF (ofs > MB_OFS_MASK);

    ofs |= (n << MB_N_SHIFT);

    mb->outb [mb->outb_len] = ofs;
    mb->outb [mb->outb_len + 1] = ofs >> 8;
    mb->outb_len += 2;
}
