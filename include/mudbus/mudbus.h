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
 *
 *      The implementation does not impose any limits on the number of instances
 *      used. Every MudBus instance (which is encapsulated in a mudbus_t strcuture)
 *      contains a field called 'driver' which contains the hardware-specific part.
 *
 *      The 'driver' field is initialized separately (see mudbus-stm32-gen.h
 *      for an easy way to generate driver initialization code for any UART).
 *
 *      There is no difference, from implementation point of view, between a
 *      bus master or a slave. The whole difference is in the fact when a device
 *      is permitted to send (master can send when the bus becomes idle, slaves
 *      send in reply to master request).
 */

#include <stdint.h>

// Include the platform-dependent library
#ifdef MCU_STM32
#include "stm32/driver.h"
#endif

/// The broadcast MudBus bus address
#define MB_BUSA_BROADCAST	0x00

/// Configuration Address Space (CAS) addresses
#define MB_CASA_DEVID		0xFFF

/// CRC8 initialization vector (0xFF after 8 rounds)
#define MB_CRC8_INIT		0xAC
/// CRC8 polynom: 2^8 + 2^5 + 2^4 + 2^0
#define MB_CRC8_POLY		0x31

/// Packet header size
#define MB_HDR_LEN		3

/// Command bitmask
#define MB_CMD_MASK		0xF0
/// Number of bits to shift command code
#define MB_CMD_SHIFT		4
/// Packet length bitmask
#define MB_LEN_MASK		0x0F

/// The mask to separate number of bytes
#define MB_N_MASK		0xF000
/// Number of positions to shift N to the right
#define MB_N_SHIFT		12
/// Maximum allowed N
#define MB_N_MAX		14
/// The mask for configuration space offset
#define MB_OFS_MASK		0x0FFF

/// Standard command codes
enum
{
    /// Error info packet
    MBC_ERROR = 0 << MB_CMD_SHIFT,
    /// Read data
    MBC_READ = 1 << MB_CMD_SHIFT,
    /// Write data
    MBC_WRITE = 2 << MB_CMD_SHIFT,
    /// Data block
    MBC_DATA = 3 << MB_CMD_SHIFT,
    /// Execute function
    MBC_EXEC = 4 << MB_CMD_SHIFT,
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
    /// Data too big
    MBE_2BIG = 4,
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

/**
 * The standard configuration address space structure
 * at offset MB_BUSA_DEVID.
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
} PACKED mb_devid_t;

/// Create a version number from (hi,lo) parts
#define MB_VERSION(h,l)		(((h & 0x0F) << 4) | (l & 0x0F))

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

/**
 * Update the CRC8 value with a new data byte.
 * @arg crc8
 *      The previous value of CRC8
 * @arg c
 *      The incoming character
 * @return
 *      The updated CRC8
 */
extern uint8_t mb_crc8_update (uint8_t crc8, uint8_t c);

/// This array can hold any legal MudBus packet
typedef uint8_t mb_packet_t [3 + 16 + 1];

/**
 * This structure holds the whole state of the MudBus master interface.
 */
typedef struct
{
    /// The platform-dependent driver
    mudbus_driver_t driver;
    /// MudBus state flags (see MBF_XXX)
    uint8_t flags;
    /// Length of valid data in incoming buffer
    uint8_t inb_len;
    /// Outgoing buffer length
    uint8_t outb_len;
    /// Device address on bus
    uint8_t busa;
    /// Outgoing packet buffer
    mb_packet_t outb;
    /// Incoming packet buffer
    mb_packet_t inb;
} mudbus_t;

/// The out buffer is transmitting
#define MBF_TX_OUTB		0x01
/// Sending 4 spaces
#define MBF_TX_SPACE		0x02
/// User code is handling an incoming packet
#define MBX_RX_DIGEST		0x08

/**
 * Initialize the MudBus instance.
 *
 * You MUST initialize the 'driver' field in the 'mudbus_t' structure
 * before calling this function. See "mudbus-stm32-gen.h" for an
 * easy way to create a driver initialization routine.
 * @arg mb
 *      The MudBus object
 * @arg busa
 *      Device bus address
 */
extern void mb_init (mudbus_t *mb, uint8_t busa);

/**
 * Check if a new packet can be sent.
 * @arg mb
 *      The MudBus object
 * @return
 *      false if there is still unsent data in the outgoing buffer.
 */
static inline bool mb_can_send (mudbus_t *mb)
{ return mb->outb_len == 0; }

/**
 * Add a fragment of the packet into the outgoing packet buffer.
 * You can modify mb->outb directly, but this function makes additional
 * checks, won't allow buffer overruns etc.
 * @arg mb
 *      The MudBus object
 * @arg data
 *      A pointer to data fragment to add into the output packet buffer.
 *      This can be NULL if len is zero.
 * @arg len
 *      The length of the packet fragment
 */
extern void mb_send_frag (mudbus_t *mb, const uint8_t *data, uint8_t len);

/**
 * Put the last fragment of the packet into the outgoing buffer
 * (can be the whole packet at once as well). The function will
 * finalize the packet (compute the CRC8) and start sending.
 *
 * Before sending a packet you may check if the transmitter
 * is ready to send a new packet with the mb_can_send() function.
 * @arg mb
 *      The MudBus object
 * @arg data
 *      A pointer to packet fragment data. This can be NULL if len is zero.
 * @arg len
 *      The length of the packet fragment
 */
extern void mb_send_last (mudbus_t *mb, const uint8_t *data, uint8_t len);

/**
 * Create the header fragment of the packet and put it in the queue.
 * @arg mb
 *      The MudBus object
 * @arg busa
 *      Target device bus address
 * @arg cmd
 *      Command code
 * @arg len
 *      The length of the data field of the packet. From 1 to 16 bytes.
 */
extern void mb_send_hdr (mudbus_t *mb, uint8_t busa, uint8_t cmd, uint8_t len);

/**
 * Synthesize a NOfs value and put it into the outgoing buffer.
 * @arg mb
 *      The MudBus object
 * @arg n
 *      Number of bytes to read/write (0 to 14)
 * @arg ofs
 *      The offset in configuration address space
 */
extern void mb_send_nofs (mudbus_t *mb, uint8_t n, uint32_t ofs);

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

/**
 * Set up hardware for receiving next packet fragment.
 * The user usually does not have to call this function as data
 * reception is handled automatically by MudBus code.
 * @arg mb
 *      The MudBus object
 */
extern void mb_recv_next (mudbus_t *mb);

/**
 * This function must be called from DMA IRQ handler for the channel that works
 * with USART RX when a DMA or USART error is detected, or the IDLE pseudo-char
 * is received. This will reset the receiver.
 * @arg mb
 *      The MudBus object
 */
extern void mb_recv_reset (mudbus_t *mb);

// ---------------- // User packet handling code helpers // ---------------- //

#define GET_NOFS(mb) \
    (((uint32_t)mb->inb [3]) | ((uint32_t)mb->inb [4] << 8))

/**
 * This structure describes an area within the configuration address space.
 */
typedef struct
{
    /// Area start offset (lower 12 bits), flags (upper 4 bits, see MBCAF_XXX)
    uint16_t sofs;
    /// Area end offset (lower 12 bits), eofs = sofs + area size
    uint16_t eofs;
    /** A pointer to area data (the 'const' qualifier is violated on write
     command if are is not write protected, c'est la vie) */
    const void *data;
} mb_cas_area_t;

/// Mask to extract area offset from sofs
#define MBCAF_OFS_MASK		0x0FFF
/// Area is write-protected
#define MBCAF_WP		0x1000

/**
 * When the master receives a packet, its CRC is checked and if the packet
 * is ok, this user-supplied function is invoked. The whole packet is in the
 * mb->inb array and the length is in mb->inb_len.
 * @arg mb
 *      The MudBus object
 */
extern void mb_user_recv (mudbus_t *mb);

/**
 * Send a ERROR command.
 * @arg mb
 *      The MudBus object
 * @arg err
 *      Error code
 * @arg data
 *      A pointer to additional data
 * @arg size
 *      The size of additional data (<15).
 */
extern void mb_cmd_error (mudbus_t *mb, uint8_t err, void *data, unsigned size);

/**
 * Execute the READ command (command is in the mb->inb buffer).
 * Data from the respective CAS area is sent in a DATA packet.
 * @arg mb
 *      The MudBus object
 * @arg cas
 *      A pointer to an array of CAS area descriptors.
 * @arg ncas
 *      Number of elements in the @a cas array.
 */
extern void mb_cmd_read (mudbus_t *mb, const mb_cas_area_t *cas, unsigned ncas);

/**
 * Execute the WRITE command (command is in the mb->inb buffer).
 * Data from packet is copied into the respective CAS area,
 * if it is not write protected.
 * @arg mb
 *      The MudBus object
 * @arg cas
 *      A pointer to an array of CAS area descriptors.
 * @arg ncas
 *      Number of elements in the @a cas array.
 */
extern void mb_cmd_write (mudbus_t *mb, const mb_cas_area_t *cas, unsigned ncas);

#endif // __MUDBUS_H__
