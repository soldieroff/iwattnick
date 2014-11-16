/*
    iWattnick MudBus master & slave handlers
    Copyright (C) 2009-2014 Andrew Zabolotny All Rights Reserved

    This code can be freely redistributed under the terms of
    GNU General Public License version 3 or later.
*/

#include "iwattnick.h"
#include "mudbus.h"

/// Device identifier
static const mb_devid_t mb_devid =
{
    /// firmware version, 4.4 hi/lo
    MB_VERSION (IWA_VERSION_HI, IWA_VERSION_LO),
    /// Manufacturer ID
    MBMID_ZAP,
    /// Product ID
    MBPID_ZAP_IWATTNICK,
    /// Type ID
    MBTID_OTHER,
};

/// CAS area descriptors
static const mb_cas_area_t mb_cas [] =
{
    { 0, sizeof (iwa_config), &iwa_config },
    { MB_CASA_DEVID | MBCAF_WP, MB_CASA_DEVID + sizeof (mb_devid), &mb_devid },
};

/*
 * Единый обработчик пакетов для шин Master и Slave.
 * В режиме ведущего нам, в основном, нужно обрабатывать ответы от ведомых
 * устройств (пакеты DATA). В режиме ведомого от нас требуется, в основном,
 * отвечать на запросы (команды READ и WRITE).
 */
static void mb_recv (mudbus_t *mb)
{
    switch (mb->inb [2] & MB_CMD_MASK)
    {
        case MBC_ERROR:
            break;

        case MBC_READ:
            mb_cmd_read (mb, mb_cas, ARRAY_LEN (mb_cas));
            break;

        case MBC_WRITE:
            mb_cmd_write (mb, mb_cas, ARRAY_LEN (mb_cas));
            break;

        case MBC_DATA:
            break;

        case MBC_EXEC:
            switch (mb->inb [3])
            {
                case MBF_RESET:
                    break;
            }
            break;
    }
}

// ----- // MudBus Master // ----- //

mudbus_t mbm;

#define MB_DRVINIT	mbd_master_init
#define MB_USART	MBM_USART
#define MB_VAR		mbm
#include "drvgen.h"

// ----- // MudBus Slave // ----- //

mudbus_t mbs;

#define MB_DRVINIT	mbd_slave_init
#define MB_USART	MBS_USART
#define MB_VAR		mbs
#include "drvgen.h"

// ----- // MudBus initialization // ----- //

void mudbus_init ()
{
    mbd_slave_init (&mbm.driver);
    mb_init (&mbs, IWA_BUSA_SLAVE);
    mbs.recv = mb_recv;

    mbd_master_init (&mbs.driver);
    mb_init (&mbm, MB_BUSA_MASTER);
    mbm.recv = mb_recv;
}
