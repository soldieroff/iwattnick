/*
    STM32 DMA library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include "stdfun.h"

// simple RNG from https://en.wikipedia.org/wiki/Random_number_generation

static unsigned m_w/* = 0xdeadbaba*/;   /* must not be zero, nor 0x464fffff */
static unsigned m_z/* = 0xf00dface*/;   /* must not be zero, nor 0x9068ffff */

void srand (unsigned seed)
{
    m_w = m_z = seed;
}

unsigned rand ()
{
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);

    if (m_w == 0 || m_w == 0x464fffff)
        m_w++;
    if (m_z == 0 || m_z == 0x9068ffff)
        m_z++;

    return (m_z << 16) + m_w;  /* 32-bit result */
}
