# coding: utf-8
#
# Библиотека для работы с СУ АКБ через интерфейс UART.
# Copyright (C) 2013 EnSol
#

import sys, serial, struct, time
from iwaif import *
from util import dump, CRC8
from port import *

# Скорость последовательного порта, бод
UART_SPEED = 9600

class UART (Port):
    def __init__ (self, port):
        super (UART, self).__init__ ()
        self.ser = serial.Serial (port, UART_SPEED, serial.EIGHTBITS, serial.PARITY_NONE,
            serial.STOPBITS_ONE, timeout = 0.01)
        # предполагается работа в режиме ведущего
        self.busa = 1


    def Build (self, busa, code, data):
        dl = len (data) - 1
        if (dl < 0) or (len (data) > 15):
            return None

        cmd = bytearray ()
        cmd.append (busa)
        cmd.append (self.busa)
        cmd.append (code | dl)
        cmd.extend (data)
        cmd.append (CRC8 (cmd, 0, len (cmd)))
        return cmd


    def Transmit (self, cmd):
        if self.debug & 2:
            dump ("Sending", cmd)
        self.ser.flushInput ()
        self.ser.write (str (cmd))


    def Receive (self):
        answer = bytearray ()
        recvcount = 0
        while True:
            c = self.ser.read ()
            if len (c) == 0:
                if (self.debug & 4) and (len (answer) != 0):
                    dump ("Incomplete packet", answer)
                return None

            answer.append (c)
            if len (answer) > MB_HEADER_LEN:
                dl = (answer [2] & MB_LEN_MASK) + 1
                if len (answer) >= MB_HEADER_LEN + dl + 1:
                    # Пакет прибыл полностью. Проверяем CRC8
                    if (CRC8 (answer, 0, MB_HEADER_LEN + dl + 1) != 0):
                        if (self.debug & 4):
                            dump ("Bad packet CRC8", answer)
                        return None

                    if self.debug & 1:
                        dump ("Received", answer)

                    return answer


    def Transact (self, cmd, retries=3):
        while True:
            self.Transmit (cmd)
            st = time.time ()
            while time.time () - st < 0.1:
                ans = self.Receive ()
                if ans != None:
                    break

            if (ans != None):
                return ans

            if (retries > 0):
                retries -= 1
                continue

            return None
