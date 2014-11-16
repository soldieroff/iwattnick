# coding: utf-8
#
# Протокол MudBus для iWattnick
#

# The broadcast MudBus bus address
MB_BUSA_BROADCAST	= 0x00
# Recommended bus master address
MB_BUSA_MASTER		= 0x01

# Configuration Address Space (CAS) addresses
MB_CASA_DEVID		= 0xFFF

# CRC8 initialization vector (0xFF after 8 rounds)
MB_CRC8_INIT		= 0xAC
# CRC8 polynom: 2^8 + 2^5 + 2^4 + 2^0
MB_CRC8_POLY		= 0x31

# Command bitmask
MB_CMD_MASK		= 0xF0
# Number of bits to shift command code
MB_CMD_SHIFT		= 4
# Packet length bitmask
MB_LEN_MASK		= 0x0F

# The mask to separate number of bytes
MB_N_MASK		= 0xF000
# Number of positions to shift N to the right
MB_N_SHIFT		= 12
# Maximum allowed N
MB_N_MAX		= 14
# The mask for configuration space offset
MB_OFS_MASK		= 0x0FFF

# Error info packet
MBC_ERROR = 0 << MB_CMD_SHIFT
# Read data
MBC_READ = 1 << MB_CMD_SHIFT
# Write data
MBC_WRITE = 2 << MB_CMD_SHIFT
# Data block
MBC_DATA = 3 << MB_CMD_SHIFT
# Execute function
MBC_EXEC = 4 << MB_CMD_SHIFT

# No error
MBE_NONE = 0
# Bad address
MBE_BADR = 1
# Access denied
MBE_ACCESS = 2
# General fault
MBE_FAULT = 3
# Data too big
MBE_2BIG = 4

#-- Standard function codes --#

# Reset device
MBF_RESET = 255

#--  Known manufacturer identifiers, globally unique --#

MBMID_ZAP = 'Z'

#-- Known product IDs: unique within a single MID --#

# iWattnick
MBPID_ZAP_IWATTNICK = 'W'

#-- Known device types, globally unique --#

# The "type does not matter" type
MBTID_OTHER = '\0'


# Supported firmware version hi
IWA_VERSION_HI		= 0
# Firmware version lo
IWA_VERSION_LO		= 1

# iWattnick address on the slave MudBus
IWA_BUSA_SLAVE		= 64


# Header length
MB_HEADER_LEN		= 3
# Max data length
MB_MAX_DATA_LEN		= 16
# Max packet length
MB_MAX_PACKET_LEN	= MB_HEADER_LEN + MB_MAX_DATA_LEN + 1


from cas import CASA

# Область Идентификатора Устройства в Конфигурационном Адресном Пространстве
# по смещению MB_CASA_DEVID
CASA_DEVID = CASA (MB_CASA_DEVID, (
    # Версия прошивки устройства
    ( "version", "B" ),
    # Идентификатор производителя (MID)
    ( "mid", "B" ),
    # Идентификатор устройства (PID)
    ( "pid", "B" ),
    # Идентификатор типа устройства (TID)
    ( "tid", "B" ),
))

CAS_ANY = [ CASA_DEVID ]
CAS_IWATTNICK = [ CASA_DEVID ]


MID2STR = {
    MBMID_ZAP: "zap",
}

PID2STR = {
    (MBMID_ZAP + MBPID_ZAP_IWATTNICK): "iWattnick",
}

TID2STR = {
    MBTID_OTHER: "Other",
}
