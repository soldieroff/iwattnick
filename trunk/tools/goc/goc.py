#!/usr/bin/python
#
# YAGL Graphics Objects Compiler
#

import sys, os, codecs
from optparse import OptionParser;


# This function is specific to the actual framebuffer format
# of your LCD/LED display. As compiler parse bitmaps, it puts
# the image in the top-down orientation, starting from bit 0:
#
# Compiler-internal numeric representation of the bitmap:
# 1022, 1153, 1285, 1393, 1285, 1153, 1022
#
# bit image
#  0  0111110
#  1  1000001
#  2  1010101
#  3  1000001
#  4  1001001
#  5  1001001
#  6  1001001
#  7  1100011
#  8  1011101
#  9  1000001
# 10  0111110
#
# Currently, PrepareBitmap transforms it this way:
#
# byte bit image
#   0   0  0111110
#       1  1000001
#       2  1010101
#       3  1000001
#       4  1001001
#       5  1001001
#       6  1001001
#       7  1100011
#   1   0  1011101
#       1  1000001
#       2  0111110
#
def PrepareBitmap (bitmaps):
    r = []
    # Loop for every frame of the bitmap
    for b in bitmaps:
        nb = []
        h = 0
        # Loop for every column of the bitmap
        for c in b:
            nc = []
            # Split numbers into bytes
            if c == 0:
                nc.append ([0])
            else:
                while c:
                    if False:
                        bit = 128
                        rv = 0
                        # Loop for every of 8 bits
                        while bit:
                            if c & 1:
                                rv |= bit
                            bit >>= 1
                            c >>= 1
                    else:
                        rv = c & 255
                        c >>= 8
                    nc.append (rv)

            nb.append (nc)
            if h < len (nc):
                h = len (nc)

        # Fill all columns with missing zeros
        for nc in nb:
            while len (nc) < h:
                nc.append (0)
        r.append (nb)

    return r


def WriteByteArray (outf, arr):
    if type (arr [0]) == list:
        newarr = []
        for x in arr:
            newarr.extend (x)
        arr = newarr
    else:
        arr = arr[:]

    while len (arr):
        x = arr [0:16]
        del arr [0:16]
        outf.write ("\t.byte\t" + ", ".join (str (y) for y in x) + "\n")


def UnescapeChar (s, p):
    if s [p + 1] == '\\':
        return '\\', p + 2

    elif "123456789".find (s [p + 1]) >= 0:
        n = 0
        e = p + 1
        while e < len (s):
            k = "0123456789".find (s [e])
            if k < 0:
                break
            n = n * 10 + k
            e += 1
        return unichr (n), e

    elif s [p + 1] == '0':
        n = 0
        e = p + 2
        while e < len (s):
            k = "01234567".find (s [e])
            if k < 0:
                break
            n = n * 8 + k
            e += 1
        return unichr (n), e

    elif s [p + 1] == 'x':
        n = 0
        e = p + 2
        while e < len (s):
            k = "0123456789ABCDEF".find (s [e].upper ())
            if k < 0:
                break
            n = n * 16 + k
            e += 1
        return unichr (n), e

    else:
        return s [p + 1], p + 2


def Unescape (s):
    p = 0
    while True:
        p = s.find ('\\', p)
        if p < 0:
            return s

        c, np = UnescapeChar (s, p)
        s = s [:p] + c + s [np:]

        p += len (c)


class SourceLine:
    def __init__(self, s):
        self.s = s


    def GetIndent (self):
        x = len (self.s)
        self.s = self.s.lstrip ()
        return x - len (self.s)


    def Indent (self, level):
        self.s = ' ' * level + self.s


    def GetToken (self):
        x = self.s.lstrip ().split (' ', 1)
        if len (x) > 1:
            self.s = x [1]
        else:
            self.s = ""

        return x [0].replace ("\\#", "#")


    def ParseBits (self, bitmap, bit):
        self.s = self.s.lstrip ()
        if not self.s:
            return False

        bframe = 0
        bcol = 0
        while self.s:
            c = self.s [0]
            self.s = self.s [1:]
            if (c != '.') and (c != '@'):
                if c.isspace ():
                    bframe += 1
                    bcol = 0
                    continue

                self.s = c + self.s
                return False

            if bit == 0:
                if bcol == 0:
                    bitmap.append ([])
                bitmap [bframe].append (0)

            if c == '@':
                bitmap [bframe][bcol] |= (1 << bit)
            bcol += 1

        return True


    def GetString (self):
        r = self.s.strip ()
        self.s = ""
        return r


class SourceFile:
    def __init__ (self, fn, enc):
        self.fn = fn
        self.enc = enc
        self.inf = open (fn, "r")
        self.lineno = 0
        self.eof = False
        self.nextline = []


    def GetLine (self, inf, compiler):
        self.lineno += 1

        if len (self.nextline):
            return self.nextline.pop ()

        try:
            l = unicode (self.inf.readline (), self.enc)

        except UnicodeDecodeError:
            compiler.Fail (inf, "Invalid character in input stream");
            l = None

        if not l:
            self.eof = True
            return None

        l = l.rstrip ("\n\r").expandtabs ()
        if not l:
            return None

        hashpos = l.find ('#')
        if hashpos >= 0:
            if (hashpos == 0) or (l [hashpos - 1] != '\\'):
                l = l [:hashpos]
        return SourceLine (l)


    def PutLine (self, line):
        self.nextline.append (line)
        self.lineno -= 1
        self.eof = False


class Object (object):
    id = None

    def CName (self):
        if len (self.id) == 1:
            return "GLYPH_%d" % ord (self.id [0])

        return "%s_%s" % (self.type, self.id)


class ObjBitmap (Object):
    type = "BITMAP"

    def __init__ (self, id, bitmap):
        self.id = Unescape (id)
        self.bitmap = PrepareBitmap (bitmap) [0]
        self.w = len (self.bitmap)
        self.h = len (self.bitmap [0]) * 8


    def Write (self, outf, compiler):
        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        if (self.w <= 0) or (self.w > 64):
            compiler.Fail (outf.name, "invalid width %d of bitmap '%s'" % (bw, self.id))
        if (self.h <= 0) or (self.h > 32):
            compiler.Fail (outf.name, "invalid height %d of bitmap '%s'" % (bh, self.id))
        # Write out the width/height
        outf.write ("\t.byte\t(%d<<6)|%d\n" % ((self.h / 8) - 1, self.w - 1))
        WriteByteArray (outf, self.bitmap)


class ObjAnim (Object):
    type = "ANIM"

    def __init__ (self, id, delay, bitmap):
        self.id = Unescape (id)
        self.delay = delay
        self.bitmap = PrepareBitmap (bitmap)


    def Write (self, outf, compiler):
        # find out max frame w/h
        #w = 0
        #h = 0
        #for bitmap in self.bitmap:
        #    bw = len (bitmap)
        #    bh = len (bitmap [0]) * 8
        #    if bw > w:
        #        w = bw
        #    if bh > h:
        #        h = bh

        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        # Write out the maximal width/height
        #outf.write ("\t.byte\t(%d<<6)|%d\n" % ((h / 8) - 1, w - 1))
        outf.write ("\t.byte\t%d\n" % (len (self.bitmap) - 1))
        outf.write ("\t.byte\t%d\n" % (self.delay - 1))
        for bitmap in self.bitmap:
            bw = len (bitmap)
            bh = len (bitmap [0]) * 8
            if (bw <= 0) or (bw > 64):
                compiler.Fail (outf.name, "invalid width %d of bitmap '%s'" % (bw, self.id))
            if (bh <= 0) or (bh > 32):
                compiler.Fail (outf.name, "invalid height %d of bitmap '%s'" % (bh, self.id))
            outf.write ("\t.byte\t(%d<<6)|%d\n" % ((bh / 8) - 1, bw - 1))
            WriteByteArray (outf, bitmap)


# TEXT flags
TF_VERTICAL = 0x80

class ObjText (Object):
    type = "TEXT"


    def __init__ (self, id, text, spacing, flags):
        self.id = Unescape (id)
        self.text = text
        self.spacing = spacing
        self.flags = flags


    def Write (self, outf, compiler, glyph_index, encoding):
        # Find out the indices of every text character
        i = 0
        res = []
        while i < len (self.text):
            if self.text [i] != '\\':
                ref = self.text [i]
                i += 1
            elif self.text [i + 1] == '{':
                try:
                    e = self.text.index ('}', i + 2)
                    ref = self.text [i + 2 : e]
                    i = e + 1
                except ValueError:
                    compiler.Fail (outf.name,
                        "Wrong object reference in text '%s':%d" % (self.text, i))
            else:
                ref, i = UnescapeChar (self.text, i + 1)


            idx = None
            for x in glyph_index:
                if (x != None) and (x.id == ref):
                    idx = x.index
                    break

            if idx is None:
                compiler.Fail (outf.name,
                    "Undefined object '%s' referenced in text '%s':%d" % (ref, self.text, i))

            res.append (idx)

        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        compiler.AddMacro (self.CName (), cname, u"\"%s\"" % x.id)
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        outf.write ("\t.byte\t%d|%d\n" % (self.flags & TF_VERTICAL, self.spacing & 127))
        outf.write ("\t.byte\t%d\n" % (len (res) - 1))
        WriteByteArray (outf, res)


class ObjAlias (Object):
    type = "ALIAS"


    def __init__ (self, id, refid):
        self.id = Unescape (id)
        self.refid = refid


    def Resolve (self, ref, compiler):
        self.ref = ref


class ObjVariable (Object):
    type = "VAR"


    def __init__ (self, id):
        self.id = Unescape (id)


class Compiler:
    # The first filename on command line
    fn = None
    # A list of bitmaps
    bitmaps = []
    # A list of animations
    anims = []
    # A list of TEXT objects
    texts = []
    # A list of ALIAS objects
    aliases = []
    # The list of included files (to avoid including twice)
    included = []
    # External symbol definitions
    externs = []
    # Macros definitions
    macros = []
    # Variables
    variables = []

    def Parse (self, fn, enc):
        if self.fn is None:
            self.fn = fn

        try:
            inf = SourceFile (fn, enc)
        except IOError:
            self.Fail (fn, "failed to open file")

        while not inf.eof:
            l = inf.GetLine (inf, self)
            if not l:
                continue

            t = l.GetToken ()
            if not t:
                continue

            elif t == "include":
                fid = l.GetToken ()
                if not fid:
                    self.Fail (inf, "expected include file name")
                if not fid in self.included:
                    self.included.append (fid)
                    self.Parse (self.FindFile (fid), enc)

            elif t == "bitmap":
                bid = l.GetToken ()
                if not bid:
                    self.Fail (inf, "expected bitmap identifier")
                self.ParseBitmap (inf, bid)

            elif t == "anim":
                aid = l.GetToken ()
                if not aid:
                    self.Fail (inf, "expected anim identifier")
                delay = l.GetToken ()
                if not delay:
                    self.Fail (inf, "expected frame delay")
                self.ParseAnim (inf, aid, delay)

            elif t == "alias":
                aid = l.GetToken ()
                if not aid:
                    self.Fail (inf, "expected alias identifier")
                bid = l.GetToken ()
                if not bid:
                    self.Fail (inf, "expected object id")
                self.ObjAdd (inf, self.aliases, ObjAlias (aid, bid))

            elif t == "text":
                tid = l.GetToken ()
                if not tid:
                    self.Fail (inf, "expected text identifier")
                spc = l.GetToken ()
                if not spc:
                    spc = 1
                    flags = None
                else:
                    flags = l.GetToken ()
                text = inf.GetLine (inf, self)
                if text is None:
                    self.Fail (inf, "a text line is expected after TEXT")
                self.ParseText (inf, tid, text.GetString (), spc, flags)

            elif t == "var":
                vid = l.GetToken ()
                if not vid:
                    self.Fail (inf, "expected variable identifier")
                if len (vid) < 2:
                    self.Fail (inf, "do not use single-char var identifiers")
                self.ObjAdd (inf, self.variables, ObjVariable (vid))

            else:
                self.Fail (inf, "unknown keyword `%s'" % t)

            if l.s:
                self.Fail (inf, "excess characters in line: `%s'" % l.s)


    def Fail (self, inf, msg):
        if isinstance (inf, SourceFile):
            print "%s:%d %s" % (inf.fn, inf.lineno, msg)
        else:
            print "%s: %s" % (inf, msg)
        sys.exit (-1)


    def FindFile (self, fn):
        for p in opts.IncludePath:
            r = os.path.join (p, fn)
            if os.access (r, os.F_OK):
                return r
        return fn


    def ObjAdd (self, inf, lst, obj):
        for x in lst:
            if x.id == obj.id:
                self.Fail (inf, "Duplicate %s identifier: '%s'" % (obj.type, obj.id))
        lst.append (obj)


    def ParseBitmap (self, inf, bid):
        bitmap = []
        bit = 0
        while not inf.eof:
            l = inf.GetLine (inf, self)
            if not l:
                break

            if bit > 31:
                self.Fail (inf, "BITMAP cannot contain more than 32 bits per column")
            try:
                if not l.ParseBits (bitmap, bit):
                    if l.s:
                        self.Fail (inf, "unexpected characters in BITMAP: `%s'" % l.s)
                    break
            except IndexError:
                self.Fail (inf, "all BITMAP lines must have same length and structure")

            bit += 1

        if len (bitmap) > 1:
            self.Fail (inf, "BITMAP must contain just one frame: %d" % len (bitmap))
        if len (bitmap) <= 0:
            self.Fail (inf, "empty BITMAP")

        self.ObjAdd (inf, self.bitmaps, ObjBitmap (bid, bitmap))


    def ParseAnim (self, inf, aid, delay):
        try:
            delay = int (delay)
        except ValueError:
            self.Fail (inf, "ANIM delay must be a integer, got `%s'" % (str (delay)))

        if (delay <= 0) or (delay > 256):
            self.Fail (inf, "ANIM delay must be in range 1..256: %d" % delay);

        anim = []
        bit = 0
        while not inf.eof:
            l = inf.GetLine (inf, self)
            if not l:
                break

            if bit > 31:
                self.Fail (inf, "ANIM cannot contain more than 32 pixels per column")
            try:
                if not l.ParseBits (anim, bit):
                    if l.s:
                        self.Fail (inf, "unexpected characters in ANIM: `%s'" % l.s)
                    break
            except IndexError:
                self.Fail (inf, "all ANIM lines must have same length and structure")

            bit += 1

        if len (anim) > 256:
            self.Fail (inf, "ANIM can't contain more than 256 frames: %d" % len (anim))
        if len (anim) <= 0:
            self.Fail (inf, "empty ANIM")

        self.ObjAdd (inf, self.anims, ObjAnim (aid, delay, anim))


    def ParseText (self, inf, tid, text, spacing, flags):
        try:
            spacing = int (spacing)
        except ValueError:
            self.Fail (inf, "TEXT spacing must be a integer, got `%s'" % (spacing.encode ('utf-8')))

        if (spacing < -64) or (spacing > 63):
            self.Fail (inf, "TEXT spacing must be in range -64..63: %d" % spacing);

        fl = 0
        if flags != None:
            for x in flags.upper ():
                if x == 'V':
                    fl |= TF_VERTICAL
                else:
                    self.Fail (inf, "Unknown TEXT flags: `%s'" % (x))

        self.ObjAdd (inf, self.texts, ObjText (tid, text, spacing, fl))


    def AddExtern (self, sym, typ, comment):
        s = u"extern %s %s[];\t/* %s */" % (typ, sym, comment)
        self.externs.append (s.encode ("utf-8"));


    def AddMacro (self, macro, value, comment):
        if type (value) != unicode:
            value = unicode (value)
        s = u"#define %s\t%s\t/* %s */" % (macro, value, comment)
        self.macros.append (s.encode ("utf-8"))


    def Write (self, opts):
        path,name = os.path.split (self.fn)
        name,ext = os.path.splitext (name)
        if path == "":
            path = "."
        fncomp = {"path" : path, "name" : name, "ext" : ext}

        name = fn.rsplit (".") [0]
        outfn = opts.Output % fncomp
        hdrfn = opts.Header % fncomp

        outf = file (outfn, "w")

        outf.write ("\t.text\n\n")

        # Resolve all aliases
        for x in self.aliases:
            for y in self.bitmaps + self.anims:
                if y.id == x.refid:
                    x.Resolve (y, self)

            if not hasattr (x, 'ref'):
                self.Fail (outfn, "Failed to resolve alias '%s' to '%s'" % (x.id, x.refid))

        # Assign indices to all objects.
        glyph_index = [None] * 256

        # First insert single-char objects to respective positions
        for x in self.bitmaps + self.anims + self.aliases + self.variables:
            setattr (x, "index", -1)
            if len (x.id) == 1:
                try:
                    idx = ord (x.id.encode (opts.OutEnc))
                    if glyph_index [idx] != None:
                        self.Fail (outfn, "Duplicate object ID: '%s'" % x.id)
                    x.index = idx
                    glyph_index [idx] = x
                    self.AddMacro (x.CName (), idx, u"\"%s\"" % x.id)

                except UnicodeDecodeError:
                    self.Fail (outfn, "Failed to encode identifier '%s'" % x.id)

        # Now insert all other objects into free slots
        free_idx = 0
        for x in self.bitmaps + self.anims + self.aliases + self.variables:
            if x.index == -1:
                while (free_idx < len (glyph_index)) and (glyph_index [free_idx] != None):
                    free_idx += 1

                if free_idx >= len (glyph_index):
                    self.Fail (outfn, "Too many glyph objects defined (256 maximum)")

                x.index = free_idx
                glyph_index [free_idx] = x
                self.AddMacro (x.CName (), free_idx, u"\"%s\"" % x.id)
                free_idx += 1

        # Finally, write the index into output file
        for n in range (255, 0, -1):
            if glyph_index [n] != None:
                del glyph_index [n+1:]
                break

        outf.write ("\t.globl gbc_glyphs\ngbc_glyphs:\n")
        for n in range (len (glyph_index)):
            if (glyph_index [n] == None) or \
               (type (glyph_index [n]) is ObjVariable):
                s = "0"
            elif type (glyph_index [n]) is ObjAlias:
                s = "_" + glyph_index [n].ref.CName () + "-gbc_glyphs"
            else:
                s = "_" + glyph_index [n].CName () + "-gbc_glyphs"
            outf.write ("\t.short\t%s\n" % s)

        # Write out the glyph type bitmap
        glyph_types = [0] * ((len (glyph_index) + 7) / 8)
        for n in range (len (glyph_index)):
            if type (glyph_index [n]) == ObjAnim:
                glyph_types [n / 8] |= 1 << (n & 7);

        outf.write ("\t.globl gbc_glyph_types\ngbc_glyph_types:\n")
        WriteByteArray (outf, glyph_types);

        # Write out the bitmaps and animations
        for x in self.bitmaps + self.anims:
            x.Write (outf, self)

        # Write out the TEXT objects
        for x in self.texts:
            x.Write (outf, self, glyph_index, opts.OutEnc)

        # Don't leave IP at odd address, this leads to strange linker errors
        outf.write (".align 2\n")
        outf.close ()

        # Generate the C header file
        if hdrfn != "":
            hdrf = file (hdrfn, "w")

            hdrf.write (
"""/* File auto-generated by gbc, do not modify */
#ifndef __%(name)s_h__
#define __%(name)s_h__

#include <stdint.h>
""" % fncomp)

            hdrf.write ("""
/* Glyph indexes in the gbc_glyphs[] table */
""")
            hdrf.write ("\n".join (self.macros))
            hdrf.write ("""

/* Definitions of tables and graphics objects */
""")
            hdrf.write ("\n".join (self.externs))

            hdrf.write ("""

#endif /* __%(name)s_h__ */
""" % fncomp)

            hdrf.close ()


parser = OptionParser (usage="%prog [-o FILE.S] FILE1.go",
                       version="%prog 0.0.1 Graphics Objects Compiler")
parser.add_option("-o", "--output", dest="Output", default="%(path)s/%(name)s.S",
                  help="write output to FILE", metavar="FILE")
parser.add_option("-H", "--header", dest="Header", default="%(path)s/%(name)s.h",
                  help="generate C header FILE", metavar="FILE")
parser.add_option("-I", "--include", dest="IncludePath",
                  default=None, action="append",
                  help="append DIR to include path list", metavar="DIR")
parser.add_option("-e", "--input-encoding", dest="InEnc", default="utf-8",
                  help="set encoding for input script files", metavar="ENC")
parser.add_option("-E", "--output-encoding", dest="OutEnc", default="cp1251",
                  help="set encoding for indexing glyph bitmaps", metavar="ENC")

(opts, args) = parser.parse_args ()

if len (args) == 0:
    parser.print_help ()
    sys.exit (-1)

if not opts.IncludePath:
    opts.IncludePath = []

c = Compiler ()
for fn in args:
    c.Parse (fn, opts.InEnc)
c.Write (opts)
