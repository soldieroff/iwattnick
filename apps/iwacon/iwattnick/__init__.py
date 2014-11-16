# coding: utf-8
#
# Module initialization
#

import os
import locale

VERSION = "0.0.1"

# Detect filename encoding: this exactly mimics the behaviour of glib
FNENC = os.getenv ("G_FILENAME_ENCODING")
if not FNENC:
    if os.getenv ("G_BROKEN_FILENAMES") or os.name == "nt":
        FNENC = locale.getpreferredencoding ()
    else:
        FNENC = "UTF-8"
