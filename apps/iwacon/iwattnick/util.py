# coding: utf-8
#
# Разные полезные классы и подпрограммы
#

import struct
import sys
import array
from iwaif import *

# Хранилище атрибутов, можно ссылкаться как по obj["attr"] так и по obj.attr
class AttrStorage:
    def __getitem__ (self, y):
        return self.__dict__ [y]

    def __setitem__ (self, y, x):
        self.__dict__ [y] = x

    def keys (self):
        return self.__dict__.keys ()

    def update (self, z):
        if z is None:
            return
        self.__dict__.update (z)


# Вспомогательный класс для распаковки бинарных данных в читабельные структуры
def PrettyUnpack (data, fmt, fields):
    data = struct.unpack (fmt, str (data))
    rv = AttrStorage ()
    for x in range (len (data)):
        setattr (rv, fields [x], data [x])
    return rv


def CRC8 (data, start, length):
    crc8 = MB_CRC8_INIT
    while length > 0:
        crc8 ^= data [start]
        for i in range (8):
            if crc8 & 0x80:
                crc8 = ((crc8 << 1) & 0xff) ^ MB_CRC8_POLY
            else:
                crc8 = (crc8 << 1) & 0xff
        start += 1
        length -= 1
    return crc8


def dump (header, data):
    if (type (data) != list) and (type (data) != bytearray) and (type (data) != array.array):
        sys.stdout.write ("%s: %s\n" % (header, str (data)))
        return

    sys.stdout.write (header)
    for x in range (len (data)):
        if ((x & 15) == 0):
            sys.stdout.write ("\n%04x " % x)
        sys.stdout.write (" %02x" % data [x])
    sys.stdout.write ("\n")


# Преобразовать пару (смещение, длина) в 16-битное значение, используемое в протоколе
def nofs (ofs, n):
    return bytearray ([ofs & 0xff, ((ofs >> 8) & 0xf) | ((n - 1) << 4)])
