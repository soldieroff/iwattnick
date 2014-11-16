# coding: utf-8
#
# Подпрограммы для работы с числами с фиксированной точкой
#

import os, re
import port_uart

if os.name == "nt":
    from scan_windows import *
else:
    from scan_posix import *


def Open (port):
    return port_uart.UART (port)


def ListPorts ():
    for order, port, name, hwid in sorted (comports ()):
        yield port
