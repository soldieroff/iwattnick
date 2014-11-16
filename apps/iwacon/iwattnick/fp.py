# coding: utf-8
#
# Подпрограммы для работы с числами с фиксированной точкой
#

import math

"""
Преобразовать число с фиксированной точкой в строковое представление
n - само число, fb - число бит в дробной части, nfrac (если задано) -
число цифр в дробной части строкового представления. Если nfrac
не задан, он вычисляется автоматически по fb.
"""
def fp2str (n, fb, nfrac = None):
    n = long (n)
    one = 1 << fb
    if nfrac is None:
        nfrac = math.ceil (math.log10 (one))
    fmt = "%d.%0" + str (int (nfrac)) + "d"
    if n < 0:
        n = -n
        fmt = "-" + fmt
    return fmt % (n >> fb, (long (pow (10, nfrac)) * (n & (one - 1))) >> fb)


"""
Преобразовать число с плавающей точкой в число с фиксированной точкой.
n - само число, ib - integer bits, число бит в целой части включая знак,
fb - fractional bits, число бит в дробной части.
"""
def f2fp (n, ib, fb, signed=False):
    if n < 0:
        if not signed:
            raise ValueError ("Fixed-point value out of range")
        val = (1 << (ib + fb)) - round ((-n) * (1 << fb))
    else:
        val = round (n * (1 << fb))

    if (val < 0) or (val > 0xffffffff):
        raise ValueError ("Fixed-point value out of range")

    return long (val)


"""
Преобразовать строковое представление числа с плавающей точкой в число
с фиксированной точкой.
"""
def str2fp (s, ib, fb, signed=False):
    return f2fp (float (s), ib, fb)
