/*
    MudBus CRC8 routine implementation
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "mudbus.h"

#if 0

// Substantialy slower but 24 bytes smaller
uint8_t mb_crc8_update (uint8_t crc8, uint8_t c)
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

uint8_t mb_crc8_update (uint8_t crc8, uint8_t c)
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
