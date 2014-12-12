# coding: utf-8
#
# Абстрактный класс, представляющий из себя порт ввода/вывода для общения с СУ АКБ
# Copyright (C) 2014 EnSol
#

# Включить побайтовый показ протокола (глобальный выключатель)
# Бит 0 - показывать входящие пакеты
# Бит 1 - показывать исходящие пакеты
# Бит 2 - показывать мусор на линии
# Бит 3 - показывать повторные передачи
DEBUG = 0

class Port (object):
    def __init__ (self):
        self.debug = DEBUG