#!/usr/bin/env python

import os, sys, glob, shutil
from distutils.core import setup, Extension

VERSION = open ("VERSION").readline ().strip ()

def RemoveDir (d):
    for root, dirs, files in os.walk (d, topdown=False):
        for name in files:
            os.remove (os.path.join (root, name))
        for name in dirs:
            os.rmdir (os.path.join (root, name))
    os.rmdir (d)

script_args = sys.argv [1:]
op = None
target_windows = False
for i in range (len (script_args) - 1, -1, -1):
    if script_args [i] == "clean" or script_args [i] == "install" or script_args [i] == "build":
        op = script_args [i]
    elif script_args [i] == "--4win":
        target_windows = True
        del script_args [i]

# Create mo files:
mofiles = []
for pofile in glob.glob('po/*.po'):
    lang = os.path.splitext (os.path.basename (pofile)) [0]
    # Piggyback on the lib/ directory used by setup()
    path = "build/locale/" + lang
    if not os.path.exists(path):
        os.makedirs (path, 0755)
    mofile = path + "/iwacon.mo"
    if op == "install" or op == "build":
        print "Generating", mofile
        os.system("msgfmt %s -o %s" % (pofile, mofile))
    mofiles.append (('share/locale/' + lang + '/LC_MESSAGES', [mofile]))

# Patch module version in __init__.py
ct = open ("suakb/__init__.py").readlines ()
for ln in range (0, len (ct)):
    if ct [ln].find ("VERSION =") >= 0:
        ct [ln] = "VERSION = \"%s\"\n" % VERSION
open ("suakb/__init__.py", "w").writelines (ct)

# Also patch the spec file
ct = open ("build/iwacon.spec").readlines ()
for ln in range (0, len (ct)):
    if ct [ln].find ("Version:") >= 0:
        ct [ln] = "Version:        %s\n" % VERSION
open ("build/iwacon.spec", "w").writelines (ct)

packages = ["suakb"]
package_dir = { "suakb": "suakb" }
if target_windows:
    packages += ["usb", "usb/backend"]
    package_dir.update ({ "usb": "usb", "usb/backend": "usb/backend" })

setup (name = 'suakb',
    version = VERSION,
    description = 'A tool for setting up Infineon-based e-bike controllers',
    author = 'EnSol engineering team',
    author_email = 'info@ensol.ru',
    url = 'http://ensol.ru/',
    license = 'Proprietary',
    platforms = ['Unix', 'Windows'],
    classifiers = [
        'Development Status :: 3 - Alpha',
        'Environment :: X11 Applications',
        'Environment :: Win32 (MS Windows)',
        'Intended Audience :: End Users/Desktop',
        'License :: Proprietary',
        'Operating System :: Linux',
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: Python',
        'Topic :: System :: Hardware :: Hardware Drivers',
        ],
    packages = packages,
    package_dir = package_dir,
    scripts = ['iwacon', 'iwacon-cli' ],
    data_files = [
        ('share/iwacon', ['share/gui.xml', 'share/iwacon.svg']),
        ('share/applications', ['build/iwacon.desktop']),
        ('share/pixmaps', ['share/iwacon.svg'])
        ] + mofiles,
    long_description = """
This is the program to configure and debug the
EnSol Battery Equalizer product.
""",
    script_args = script_args
)

if op == "clean":
    RemoveDir ("build/locale")
