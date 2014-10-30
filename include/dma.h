/*
    STM32 DMA library
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __DMA_H__
#define __DMA_H__

/**
 * @file dma.h
 *      This is a set of routines that simplify DMA usage.
 *      Before starting using DMA you must enable DMA controller power
 *      by setting the appropiate bits RCC_AHBENR_DMA?EN in RCC->AHBENR.
 */

/// Universal DMA flags (all chans use same defs, so why bother to have different...)

#define  DMA_CCR_EN		DMA_CCR1_EN			/*!< Channel enable*/
#define  DMA_CCR_TCIE		DMA_CCR1_TCIE			/*!< Transfer complete interrupt enable */
#define  DMA_CCR_HTIE		DMA_CCR1_HTIE			/*!< Half Transfer interrupt enable */
#define  DMA_CCR_TEIE		DMA_CCR1_TEIE			/*!< Transfer error interrupt enable */
#define  DMA_CCR_DIR		DMA_CCR1_DIR			/*!< Data transfer direction */
#define  DMA_CCR_CIRC		DMA_CCR1_CIRC			/*!< Circular mode */
#define  DMA_CCR_PINC		DMA_CCR1_PINC			/*!< Peripheral increment mode */
#define  DMA_CCR_MINC		DMA_CCR1_MINC			/*!< Memory increment mode */

#define  DMA_CCR_PSIZE_8	0				/*!< Peripheral size 8 bits */
#define  DMA_CCR_PSIZE_16	DMA_CCR1_PSIZE_0		/*!< Peripheral size 16 bits */
#define  DMA_CCR_PSIZE_32	DMA_CCR1_PSIZE_1		/*!< Peripheral size 32 bits */

#define  DMA_CCR_MSIZE_8	0				/*!< Memory size 8 bits */
#define  DMA_CCR_MSIZE_16	DMA_CCR1_MSIZE_0		/*!< Memory size 16 bits */
#define  DMA_CCR_MSIZE_32	DMA_CCR1_MSIZE_1		/*!< Memory size 32 bits */

#define  DMA_CCR_PL_LOW		0				/*!< Channel Priority level is low */
#define  DMA_CCR_PL_MED		DMA_CCR1_PL_0			/*!< Channel Priority level is medium */
#define  DMA_CCR_PL_HIGH	DMA_CCR1_PL_1			/*!< Channel Priority level is high */
#define  DMA_CCR_PL_VERYHIGH	(DMA_CCR1_PL_1 | DMA_CCR1_PL_0)	/*!< Channel Priority level is very high */

#define  DMA_CCR_MEM2MEM	DMA_CCR1_MEM2MEM		/*!< Memory to memory mode */

/**
 * Get a pointer to the DMA controller associated with given feature.
 * Example: DMA(UART1_TX) will expand into something like DMA1
 */
#define DMA(x)			JOIN2 (DMA, DMA_NUM (x))

/**
 * Get a pointer to DMA controller channel associated with given feature.
 * Example: DMAC(UART1_TX) will expand into something like DMA1_Channel4
 */
#define DMAC(x)			JOIN4 (DMA, DMA_NUM (x), _Channel, DMA_CHAN (x))

/**
 * DMA interrupt status flag (ISR global DMA register)
 * Example: DMA_ISR (UART1_TX, GIF) -> DMA_ISR_GIF4
 */
#define DMA_ISR(x,f)		JOIN3 (DMA_ISR_, f, DMA_CHAN (x))

/**
 * DMA interrupt status clear flag (IFCR global DMA register)
 * Example: DMA_IFCR (UART1_TX, CGIF) -> DMA_IFCR_CGIF4
 */
#define DMA_IFCR(x,f)		JOIN3 (DMA_IFCR_, f, DMA_CHAN (x))

/**
 * DMA channel configuration flag (DMA_CCR)
 * Example: DMA_CCR (UART1_TX, MEM2MEM) -> DMA_CCR4_MEM2MEM
 */
#define DMA_CCR(x,f)		JOIN4 (DMA_CCR, DMA_CHAN(x), _, f)

/**
 * Declare the DMA IRQ handler for given hardware feature
 */
#define DMA_IRQ_HANDLER(x) \
void JOIN5 (DMA, DMA_NUM (x), _Channel, DMA_CHAN (x), _IRQHandler) ()

/**
 * Copy data using DMA controller.
 * You must set up the peripherial you're reading from/writing to prior to invoking
 * this function.
 * @arg dma
 *      The DMA controller to use (DMA1, DMA2, ...)
 * @arg chan
 *      DMA channel number (counting from 1)
 * @arg ccr
 *      A combination of DMA_CCR flags. You must set only *IE, CIRC, PSIZE*, MSIZE*
 *      and PL* bits, the rest are computed and set automatically.
 * @arg src
 *      The source address (memory or peripherial)
 * @arg dst
 *      The destination address (memory or peripherial)
 * @arg count
 *      Number of copied elements
 */
extern void dma_copy (DMA_TypeDef *dma, unsigned chan, uint32_t ccr,
    const void *src, volatile void *dst, unsigned count);

/**
 * This function is identical to @a dma_copy except that it works specifically with the
 * DMA1 controller. You may use it for microcontrollers with just one DMA controller
 * or if you don't need the second controller.
 * @arg chan
 *      DMA channel number (counting from 1)
 * @arg ccr
 *      A combination of DMA_CCR flags. You must set only *IE, CIRC, PSIZE*, MSIZE*
 *      and PL* bits, the rest are computed and set automatically.
 * @arg src
 *      The source address (memory or peripherial)
 * @arg dst
 *      The destination address (memory or peripherial)
 * @arg count
 *      Number of copied elements
 */
extern void dma1_copy (unsigned chan, uint32_t ccr,
    const void *src, volatile void *dst, unsigned count);

/**
 * This function is identical to @a dma_copy except that it works specifically with the
 * DMA2 controller. You may use it if you're using DMA2 extensively and you don't need
 * the general form, this will save a bit of code size by not passing a pointer to the
 * DMA controller.
 * @arg chan
 *      DMA channel number (counting from 1)
 * @arg ccr
 *      A combination of DMA_CCR flags. You must set only *IE, CIRC, PSIZE*, MSIZE*
 *      and PL* bits, the rest are computed and set automatically.
 * @arg src
 *      The source address (memory or peripherial)
 * @arg dst
 *      The destination address (memory or peripherial)
 * @arg count
 *      Number of copied elements
 */
extern void dma2_copy (unsigned chan, uint32_t ccr,
    const void *src, volatile void *dst, unsigned count);

#endif // __DMA_H__
