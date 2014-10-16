#!/usr/bin/python

import sys

if len (sys.argv) <= 1:
    sys.stderr.write ("Usage: %s [cpu] [cpu-family] [flash-size] [ram-size]" % sys.argv [0])
    sys.exit (1)

cpu_prefix = sys.argv [1][:7]

# As of today all devices use same flash & RAM start address
flash_origin = 0x08000000
ram_origin = 0x20000000

flash_size = long (sys.argv [3], 0) * 1024
ram_size = long (sys.argv [4], 0) * 1024

flash_end = flash_origin + flash_size
ram_end = ram_origin + ram_size

sys.stdout.write ("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x %s" % \
    (flash_origin, flash_size, flash_end, \
     ram_origin, ram_size, ram_end, \
     cpu_prefix))
