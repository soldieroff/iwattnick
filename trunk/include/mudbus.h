/*
    MudBus protocol
    Copyright (C) 2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU Less General Public License version 3 or later.
*/

#ifndef __MUDBUS_H__
#define __MUDBUS_H__

/**
 * @file mudbus.h
 *      MudBus is a very simple ModBus-like protocol designed to control
 *      up to 126 slave devices on a single serial bus (UART, RS-232, RS-485 etc).
 *      There is only one master and there's no way to detect bus conflicts,
 *      thus the protocol is designed to avoid them.
 */

#include <stdint.h>

// Include the platform-dependent library
#ifdef MCU_stm32
#include "mudbus-stm32.h"
#endif

/// CRC8 initialization vector (0xFF after 8 rounds)
#define MB_CRC8_INIT		0xAC
/// CRC8 polynom: 2^8 + 2^5 + 2^4 + 2^0
#define MB_CRC8_POLY		0x31

/// Error flag
#define MB_ERROR		0x80
/// Command bitmask
#define MB_CMD_MASK		0x70
/// Packet length bitmask
#define MB_LEN_MASK		0x0F

/// The mask to separate number of bytes
#define MB_N_MASK		0xF000
/// Number of positions to shift N to the right
#define MB_N_SHIFT		12
/// The mask for configuration space offset
#define MB_OFS_MASK		0x0FFF

/// Standard command codes
enum
{
    /// Read data
    MBC_READ = 0,
    /// Write data
    MBC_WRITE = 1,
    /// Execute function
    MBC_EXEC = 2,
};

/// Error codes
enum
{
    /// No error
    MBE_NONE = 0,
    /// Bad address
    MBE_BADR = 1,
    /// Access denied
    MBE_ACCESS = 2,
    /// General fault
    MBE_FAULT = 3,
};

/// Standard function codes
enum
{
    /// Reset device
    MBF_RESET = 255,
};

/**
 * Known manufacturer identifiers, globally unique.
 */
enum
{
    ///
    MBMID_ZAP = 'Z',
};

/**
 * Known product IDs: unique within a single MID.
 */
enum
{
    /// iWattnick
    MBPID_ZAP_IWATTNICK = 'W',
};

/**
 * Known device types, globally unique.
 * This is used when there are many devices from different vendors
 * with same or similar functionality. Devices with same TID will
 * have identic (or very similar) interface.
 */
enum
{
    /// The "type does not matter" type
    MBTID_OTHER = 0,
};

/// Offset to standard configuration area
#define MB_STDCONF_OFS		0xFF0

/**
 * The standard configuration address space structure
 * at offset MB_STDCONF_OFS.
 */
typedef struct
{
    /// firmware version, 4.4 hi/lo
    uint8_t Version;
    /// Manufacturer ID
    uint8_t MID;
    /// Product ID
    uint8_t PID;
    /// Type ID
    uint8_t TID;
    /// Device address on bus
    uint8_t Addr;
} PACKED mb_stdconf_t;

/**
 * Compute the CRC8 of a data block.
 * @arg data
 *      A pointer to data
 * @arg len
 *      The length of data
 * @return
 *      The computed CRC8
 */
extern uint8_t mb_crc8 (uint8_t *data, int len);

/// This array can hold any legal MudBus packet
typedef uint8_t mb_packet_t [2 + 16 + 1];

/// Max number of fragments in the outgoing queue
#define MBM_QUEUE_SIZE		4

/**
 * This structure holds the whole state of the MudBus master interface.
 */
typedef struct
{
    /// Pointers to data fragments in the outgoing queue
    const uint8_t *queue_data [MBM_QUEUE_SIZE];
    /// The length of data fragments in the outgoing queue
    uint8_t queue_len [MBM_QUEUE_SIZE];
    /// The platform-dependent driver
    mudbus_driver_t driver;
    /// MudBus Master state flags (see MBMF_XXX)
    volatile uint8_t flags;
    /// Queue head and tail pointers (if equal, see MBMF_EMPTYQ)
    volatile uint8_t queue_head, queue_tail;
    /// The current CRC8 (updated with every new block)
    volatile uint8_t crc8;
    /// Length of valid data in incoming buffer
    volatile uint8_t inb_len;
    /// Incoming packet buffer
    volatile mb_packet_t inb;
} mudbus_t;

/// Sending CRC8
#define MBMF_CRC8		0x01
/// Sending 4 spaces
#define MBMF_SPACE		0x02
/// The queue tail element has been sent
#define MBMF_TAILSENT		0x04
/// Outgoing queue is empty
#define MBMF_EMPTYQ		0x80

/**
 * Initialize the MudBus in Master mode.
 * The routine will initialize the USART defined by the MBM_USART macro
 * and the associated DMA channel.
 * @arg mb
 *      The MudBus object.
 */
extern void mbm_init (mudbus_t *mb);

/**
 * Initialize the MudBus in Slave mode.
 * The routine will initialize the USART defined by the MBS_USART macro
 * and the associated DMA channel.
 * @arg mb
 *      The MudBus object.
 */
extern void mbs_init (mudbus_t *mb);

/**
 * Put a fragment of the packet into the outgoing queue.
 * After sending the last fragment the engine will automatically
 * send a CRC8 of all the data that has been sent.
 * @arg mb
 *      The MudBus object
 * @arg data
 *      A pointer to packet fragment data
 * @arg len
 *      The length of the packet fragment
 * @return
 *      false if there's no space in queue
 */
extern bool mb_send (mudbus_t *mb, const uint8_t *data, uint8_t len);

/**
 * When the master receives a packet, its CRC is checked and if the packet
 * is ok, it is sent to this user-supplied function.
 * @arg mb
 *      The MudBus object
 * @arg data
 *      A pointer to whole packet, starting from header
 */
extern void mb_user_recv (mudbus_t *mb, uint8_t *data);

/**
 * When the master interface receives a error packet, it is sent to this
 * user-supplied function.
 * @arg mb
 *      The MudBus object
 * @arg data
 *      A pointer to whole packet, starting from header
 */
extern void mb_user_error (mudbus_t *mb, uint8_t *data);

/**
 * This function must be called from DMA IRQ handler for the channel that works
 * with USART TX at the end of transmission. The routine will initiate the
 * transmission of the next fragment, if there is any in the queue.
 * @arg mb
 *      The MudBus object
 */
extern void mb_send_next (mudbus_t *mb);

/**
 * This function must be called from DMA IRQ handler for the channel that works
 * with USART TX when a DMA or USART error is detected. This will stop all the
 * transfers and clear the queue.
 * @arg mb
 *      The MudBus object
 */
extern void mb_send_stop (mudbus_t *mb);

#endif // __MUDBUS_H__
