# coding: utf-8
#
# Работа с Конфигурационным Адресным Пространством устройств
# Copyright (C) 2013 EnSol
#

import struct
from util import AttrStorage

# Configuration Address Space Area
class CASA:
    def __init__(self, base, fields):
        self.base = base
        self.fields = fields


    def Ofs (self, field):
        curofs = self.base
        for x in self.fields:
            cursz = struct.calcsize (x [1])

            if x [0] == field:
                return curofs

            curofs += cursz

        return None


    def Length (self):
        length = 0
        for x in self.fields:
            length += struct.calcsize (x [1])

        return length


    # Возвращает [смещение] [длина] [количество распознанных полей]
    def NOfs (self, fields):
        curf = 0
        curofs = self.base
        ofs = self.base
        size = 0
        for x in self.fields:
            cursz = struct.calcsize (x [1])

            if x [0] == fields [curf]:
                if curf == 0:
                    ofs = curofs
                curf += 1
                size += cursz
                if curf >= len (fields):
                    return ofs, size, curf
            elif curf > 0:
                return ofs, size, curf

            curofs += cursz

        return 0, 0, 0


    def Iterate (self, fields, maxdatalen, data=None):
        if (data != None) and (len (fields) != len (data)):
            return

        cofs = -1
        sfields = []
        ofs = self.base
        d = None
        for x in self.fields:
            size = struct.calcsize (x [1])
            try:
                i = fields.index (x [0])
                if data != None:
                    d = data [i]
                sfields.append ([ofs, size, x [1], d])
            except ValueError:
                pass
            ofs += size

        # Сортируем поля по смещению (первый элемент массива)
        sfields.sort ()

        sofs = -1
        ofs = -1
        fmt = "<"
        dat = []
        for x in sfields:
            if (ofs != x [0]) or (ofs - sofs + x [1] > maxdatalen):
                if sofs >= 0:
                    if data != None:
                        d = bytearray (struct.pack (fmt, *dat))
                    yield sofs, ofs - sofs, d
                ofs = x [0]
                sofs = x [0]
                fmt = "<"
                dat = []
            ofs += x [1]
            fmt += x [2]
            dat.append (x [3])

        if ofs != sofs:
            if data != None:
                d = bytearray (struct.pack (fmt, *dat))
            yield sofs, ofs - sofs, d


    def Unpack (self, data, fields):
        sfields = []
        ofs = self.base
        for x in self.fields:
            size = struct.calcsize (x [1])
            if x [0] in fields:
                sfields.append ([ofs, x])
            ofs += size

        # Сортируем поля по смещению (первый элемент массива)
        sfields.sort ()

        fmt = "<"
        for x in sfields:
            fmt += x [1][1]

        ret = AttrStorage ()
        data = struct.unpack (fmt, str (data))
        for x in range (len (sfields)):
            setattr (ret, sfields [x][1][0], data [x])

        return ret
