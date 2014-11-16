# Скопируйте этот файл в local-config.mak и правьте по своему усмотрению.

# Аппаратная конфигурация (задаёт макросы для окружения микроконтроллера,
# суффикс для файла include/hardware-*.h)
#HARDWARE = stm32vl_discovery
HARDWARE = iwattnick

# Раскоментировать для отладочной компиляции (без оптимизации, с отладочной инфой)
#MODE = debug

# Если нужно, можно переопределить минимальный размер кучи (проверяется при линковке)
#STM32_MIN_HEAP_SIZE = 0
# Если нужно, можно переопределить минимальный размер стека (проверяется при линковке)
#STM32_MIN_STACK_SIZE = 0x200

# Uncomment to allow building stock STM32VL-DISCOVERY examples
#STOCK_EXAMPLES = 1
