/*
    STM32 DMA library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#include HARDWARE_H
#include "dma.h"

#ifdef DMA1_BASE
#define _DMA_NUM 1
#include "dmafun.h"
#endif

#ifdef DMA2_BASE
#define _DMA_NUM 2
#include "dmafun.h"
#endif

#if defined DMA1_BASE
#undef _DMA_NUM
#include "dmafun.h"
#endif
