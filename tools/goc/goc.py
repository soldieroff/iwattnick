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

        # Height can't be 0 (space)
        if h == 0:
            h = 1

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


def WriteBytecode (outf, code):
    for x in code:
        if not x [0].endswith (':'):
            outf.write ('\t')
        outf.write (x [0])
        if len (x) > 1:
            outf.write ('\t' + ", ".join (str (y) for y in x [1:]))
        outf.write ('\n')


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


def CSym (c):
    if (len (c) == 1) and (ord (c) < 32):
        return "\\x%x" % ord (c)
    return c


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
        self.s = self.s.strip ()
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
        r = self.s.strip (" \t")
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
    idshift = 0
    used = False


    def MarkUsed (self):
        self.used = True


    def SetID (self, id):
        id = Unescape (id)
        if len (id) > 1:
            for i in range (len (id)):
                if (ord (id [i]) > 127) or ((not id [i].isalnum ()) and (id [i] != '_')):
                    print u"Warning: invalid character '%c' in id '%s' replaced with underscore" \
                        % (id [i], id)
                    id = id [:i] + u'_' + id [i + 1:]

        self.id = id

    def CName (self):
        if len (self.id) == 1:
            if (ord (self.id) < 128) and self.id.isalnum ():
                return "GLYPH_%s" % self.id
            return "GLYPH_U%d" % ord (self.id [0])

        return "%s_%s" % (self.type, self.id)


class ObjBitmap (Object):
    type = "BITMAP"

    def __init__ (self, id, bitmap):
        self.SetID (id)
        self.bitmap = PrepareBitmap (bitmap) [0]
        self.w = len (self.bitmap)
        self.h = len (self.bitmap [0]) * 8


    def Write (self, outf, compiler):
        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        compiler.AddMacro (self.CName (), self.index, u"\"%s\"" % CSym (self.id))
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        if (self.w <= 0) or (self.w > 64):
            compiler.Fail (outf, "invalid width %d of bitmap '%s'" % (self.w, self.id))
        if (self.h <= 0) or (self.h > 32):
            compiler.Fail (outf, "invalid height %d of bitmap '%s'" % (self.h, self.id))
        # Write out the width/height
        outf.write ("\t.byte\t(%d<<6)|%d\n" % ((self.h / 8) - 1, self.w - 1))
        WriteByteArray (outf, self.bitmap)


class ObjAnim (Object):
    type = "ANIM"

    def __init__ (self, id, delay, bitmap):
        self.SetID (id)
        self.delay = delay
        self.bitmap = PrepareBitmap (bitmap)


    def Write (self, outf, compiler):
        # find out max frame w/h
        w = 0
        h = 0
        for bitmap in self.bitmap:
            bw = len (bitmap)
            bh = len (bitmap [0]) * 8
            if bw > w:
                w = bw
            if bh > h:
                h = bh

        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        compiler.AddMacro (self.CName (), self.index, u"\"%s\"" % CSym (self.id))
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        outf.write ("\t.byte\t(%d<<6)|%d\n" % ((h / 8) - 1, w - 1))
        outf.write ("\t.byte\t%d // nframes\n" % (len (self.bitmap) - 1))
        outf.write ("\t.byte\t%d // delay\n" % (self.delay - 1))
        for bitmap in self.bitmap:
            bw = len (bitmap)
            bh = len (bitmap [0]) * 8
            if (bw <= 0) or (bw > 64):
                compiler.Fail (outf, "invalid width %d of bitmap '%s'" % (bw, self.id))
            if (bh <= 0) or (bh > 32):
                compiler.Fail (outf, "invalid height %d of bitmap '%s'" % (bh, self.id))
            outf.write ("\t.byte\t(%d<<6)|%d\n" % ((bh / 8) - 1, bw - 1))
            WriteByteArray (outf, bitmap)


    def Append (self, compiler, inf, obj):
        if self.delay != obj.delay:
            compiler.Fail (inf, "ANIM with same ID '%s' but different delay %d" % (obj.id, obj.delay))

        self.bitmap.extend (obj.bitmap)


class ObjText (Object):
    type = "TEXT"


    def __init__ (self, id, text, spacing):
        self.SetID (id)
        self.text = text
        self.spacing = spacing
        self.glyphs = None
        self.refs = None


    def Write (self, outf, compiler):
        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.text)
        compiler.AddMacro (self.CName (), cname, u"\"%s\"" % self.text)
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        outf.write ("\t.byte\t%d\n" % self.spacing)
        outf.write ("\t.byte\t%d\n" % (len (self.glyphs) - 1))
        WriteByteArray (outf, self.glyphs)


    def Prepare (self, outf, compiler, glyph_index):
        # Find out the indices of every text character
        i = 0
        self.glyphs = []
        self.refs = []
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
                    compiler.Fail (outf,
                        "Wrong reference in text '%s' at pos %d" % (self.text, i))
            else:
                ref, i = UnescapeChar (self.text, i + 1)

            idx = None
            for x in glyph_index:
                if (x != None) and (x.id == ref):
                    self.refs.append (x)
                    if hasattr (x, "ref"):
                        idx = x.ref.index
                    else:
                        idx = x.index
                    break

            if idx is None:
                # Aliases could not be in glyph index, do a separate search
                for x in compiler.aliases:
                    if x.id == ref:
                        idx = x.ref.index
                        self.refs.append (x)
                        break

            if idx is None:
                # Look in the list of variables
                comma = ref.find (',')
                arg = 0
                if comma >= 0:
                    arg = ref [comma + 1:]
                    ref = ref [:comma]

                    # Handle pre-defined tags
                    if ref == 'dir':
                        vals = ['right', 'down'];
                        try:
                            arg = vals.index (arg)
                        except ValueError:
                            compiler.Fail (outf,
                                "dir must be one of " + str (vals))
                    elif ref == 'align':
                        vals = ['center', 'right'];
                        try:
                            arg = vals.index (arg)
                        except ValueError:
                            compiler.Fail (outf,
                                "align must be one of " + str (vals))
                    else:
                        for x in glyph_index:
                            if (x != None) and (x.id == arg):
                                arg = x.index
                                self.refs.append (x)
                                break

                if (type (arg) == str) or (type (arg) == unicode):
                    try:
                        # Try to interpret it as a number
                        arg = int (arg, 0)
                    except (TypeError, ValueError):
                        compiler.Fail (outf,
                            "Unknown arg '%s' for rendering variable '%s'" % (arg, ref))

                for x in compiler.variables:
                    if x.id == ref:
                        idx = [ 0, x.index, arg ]
                        self.refs.append (x)
                        break

                if idx is None:
                    compiler.Fail (outf,
                        "Unknown glyph '%s' referenced in text '%s' at pos %d" % (ref, self.text, i))

            if type (idx) == list:
                self.glyphs.extend (idx)
            else:
                self.glyphs.append (idx)


    def MarkUsed (self):
        self.used = True
        for x in self.refs:
            if not x.used:
                x.MarkUsed ()


class ObjAlias (Object):
    type = "ALIAS"


    def __init__ (self, id, refid):
        self.SetID (refid)
        self.refid = self.id
        self.SetID (id)


    def MarkUsed (self):
        self.used = True
        if not self.ref.used:
            self.ref.MarkUsed ()


    def Resolve (self, ref):
        self.ref = ref


    def Write (self, outf, compiler):
        compiler.AddMacro (self.CName (), self.ref.index, u"\"%s\"" % CSym (self.id))


class ObjVariable (Object):
    type = "VAR"


    def __init__ (self, id):
        self.SetID (id)


    def Write (self, compiler):
        compiler.AddMacro (self.CName (), self.index, u"\"%s\"" % self.id)


class ObjAction (Object):
    type = "ACTION"


    def __init__ (self, id, args):
        self.SetID (id)
        self.args = args
        self.argref = []


    def MarkUsed (self):
        self.used = True
        for x in self.argref:
            if not x.used:
                x.MarkUsed ()


    def ParseArgs (self, compiler, inf, args):
        if len (self.args) != len (args):
            compiler.Fail (inf, u"action '%s' takes %d args, got %d" % \
                (self.id, len (self.args), len (args)))

        ret = [[".byte", str (self.Value ())]]
        for n in range (len (self.args)):
            if self.args [n] == 'L':
                x = compiler.ObjFind (compiler.layouts, args [n])
                if x is None:
                    hint = ""
                    if (opts.Optimize != "") and (opts.Optimize.find ('l') == -1):
                        hint = " (possibly optimized out?)"
                    compiler.Fail (inf, u"arg %d layout '%s' not found%s" % \
                        (n + 1, args [n], hint))
                ret.append ([".sleb128", "%d" % x.index])
                self.argref.append (x)

            elif self.args [n] == 'V':
                x = compiler.ObjFind (compiler.variables, args [n])
                if x is None:
                    compiler.Fail (inf, u"arg %d expected variable id, got '%s'" % \
                        (n + 1, args [n]))
                ret.append ([".sleb128", str (x.index)])
                self.argref.append (x)

            elif self.args [n] == 'N':
                try:
                    x = int (args [n], 0)
                except (TypeError, ValueError):
                    compiler.Fail (inf, u"arg %d expected number, got '%s'" % \
                        (n + 1, args [n]))
                ret.append ([".sleb128", str (x)])

        return ret


    def Write (self, compiler):
        compiler.AddMacro (self.CName (), self.Value (), u"\"%s\"" % self.id)


    def Value (self):
        return (self.index << 2) | len (self.args)


class ObjMenu (Object):
    type = "MENU"


    def __init__ (self, id, items, itemh, yshift):
        self.SetID (id)
        self.items = items
        self.itemh = itemh
        self.yshift = yshift


    def MarkUsed (self):
        self.used = True
        for i in self.items:
            if not i [0].used:
                i [0].MarkUsed ()
            if not i [2].used:
                i [2].MarkUsed ()


    def Write (self, outf, compiler):
        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        compiler.AddMacro (self.CName (), self.index, u"\"%s\"" % self.id)
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        outf.write ("\t.byte\t%d // item count\n" % len (self.items))
        outf.write ("\t.uleb128\t%d // item height\n" % self.itemh)
        outf.write ("\t.uleb128\t%d // item y shift\n" % self.yshift)

        for i in self.items:
            # i is [act, args, txt]
            outf.write ("\t.uleb128\t_%s-goc_texts\n" % i [2].CName ())

        outf.write ("// menu item actions\n")
        for i in self.items:
            # i is [act, args, txt]
            args = i [0].ParseArgs (compiler, outf, i [1])
            WriteBytecode (outf, args)


class ObjLayout (Object):
    type = "LAYOUT"


    def __init__ (self, id, w, h, bytecode, refs):
        self.SetID (id)
        self.w = w
        self.h = h
        self.bytecode = bytecode
        self.refs = refs


    def MarkUsed (self):
        self.used = True
        for x in self.refs:
            if not x.used:
                x.MarkUsed ()


    def Write (self, outf, compiler):
        cname = "_" + self.CName ();
        compiler.AddExtern (cname, "uint8_t", u"\"%s\"" % self.id)
        compiler.AddMacro (self.CName (), self.index, u"\"%s\"" % self.id)
        outf.write ("\t.globl\t%(name)s\n%(name)s:\n" % { "name": cname })
        outf.write ("\t.uleb128\t%d,%d // width, height\n" % (self.w, self.h))

        WriteBytecode (outf, self.bytecode)


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
    # Variables
    variables = []
    # Actions
    actions = []
    # Menus
    menus = []
    # Layouts
    layouts = []
    # The list of included files (to avoid including twice)
    included = []
    # External symbol definitions
    externs = []
    # Macros definitions
    macros = []
    # Current code shift
    shift = 0
    # Temp array for finding free action indices
    action_slots = [None] * 256
    # Output files to delete on failure
    generated_files = []


    def __init__ (self):
        self.ObjAdd (None, self.variables, ObjVariable ("dir"))
        self.ObjAdd (None, self.variables, ObjVariable ("align"))


    def Parse (self, fn):
        if self.fn is None:
            self.fn = fn

        try:
            inf = SourceFile (fn, opts.InEnc)
        except IOError:
            self.Fail (fn, "failed to open file")

        while not inf.eof:
            l = inf.GetLine (inf, self)
            if not l:
                continue

            indent = l.GetIndent ()
            t = l.GetToken ()
            if not t:
                continue

            elif t == "include":
                fid = l.GetToken ()
                if not fid:
                    self.Fail (inf, "expected include file name")
                if not fid in self.included:
                    self.included.append (fid)
                    self.Parse (self.FindFile (fid))

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
                    delay = 1
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
                spc = self.GetSpacing (inf, l.GetToken ())
                text = l.GetString ()
                if len (text) == 0:
                    self.Fail (inf, "text string is missing")
                self.ObjAdd (inf, self.texts, ObjText (tid, text, spc))

            elif t == "var":
                vid = l.GetToken ()
                if not vid:
                    self.Fail (inf, "expected variable identifier")
                if len (vid) < 2:
                    self.Fail (inf, "do not use single-char var identifiers")
                self.ObjAdd (inf, self.variables, ObjVariable (vid))

            elif t == "shift":
                val = l.GetToken ()
                if not val:
                    self.Fail (inf, "expected shift value")
                try:
                    self.shift = int (val, 0)
                except ValueError:
                    self.Fail (inf, "invalid shift value '%s'" % val)

            elif t == "menu":
                mid = l.GetToken ()
                if not mid:
                    self.Fail (inf, "expected menu identifier")

                ih = l.GetToken ()
                if (not ih):
                    self.Fail (inf, "menu item height must follow the identifier")
                try:
                    ih = int (ih, 0)
                except:
                    self.Fail (inf, "menu item height must be a number, got %s" % ih)

                ys = l.GetToken ()
                if not ys:
                    ys = 0
                else:
                    try:
                        ys = int (ys, 0)
                    except:
                        self.Fail (inf, "menu item Y shift must be a number, got %s" % ys)
                self.ParseMenu (inf, indent, mid, ih, ys)

            elif t == "layout":
                lid = l.GetToken ()
                if not lid:
                    self.Fail (inf, "expected layout identifier")
                lw = l.GetToken ()
                lh = l.GetToken ()
                if (not lw) or (not lh):
                    self.Fail (inf, "layout width and height must follow the identifier")
                try:
                    lw = int (lw, 0)
                    lh = int (lh, 0)
                except:
                    self.Fail (inf, "layout width and height must be numbers, got %s and %s" % \
                        (str (lw), str (lh)))
                self.ParseLayout (inf, indent, lid, lw, lh)

            elif t == "action":
                aid = l.GetToken ()
                if not aid:
                    self.Fail (inf, "expected action identifier")
                args = (l.GetToken () or '').upper ()
                if args.strip ('NVL') != '':
                    self.Fail ("action arg types expected, got '%s'" % args)
                if len (args) > 3:
                    self.Fail ("max 3 arguments are allowed, got '%s'" % args)
                self.ObjAdd (inf, self.actions, ObjAction (aid, args))

            elif t == "used":
                otype = l.GetToken ()
                if not otype:
                    self.Fail (inf, "expected object type")
                self.Used (inf, otype, l)

            else:
                self.Fail (inf, "unknown keyword `%s'" % t)

            if l.s:
                self.Fail (inf, "excess characters in line: `%s'" % l.s)


    def Fail (self, inf, msg):
        if isinstance (inf, SourceFile):
            print "%s:%d %s" % (inf.fn, inf.lineno, msg)
        else:
            if isinstance (inf, file):
                inf = inf.name
            print "%s: %s" % (inf, msg)

        for fn in self.generated_files:
            os.unlink (fn)

        sys.exit (-1)


    def FindFile (self, fn):
        for p in opts.IncludePath:
            r = os.path.join (p, fn)
            if os.access (r, os.F_OK):
                return r
        return fn


    def ObjFind (self, lst, id):
        if id is None:
            return None

        id = Unescape (id)
        for x in lst:
            if x.id == id:
                return x
        return None


    def ObjAdd (self, inf, lst, obj):
        obj.idshift = self.shift

        if type (obj) == ObjAction:
            obj.index = 0
            while self.action_slots [obj.Value ()] != None:
                obj.index += 1
                if obj.index > 63:
                    self.Fail (inf, "Out of action identifiers: '%s'" % (obj.id))
            self.action_slots [obj.Value ()] = obj
        else:
            obj.index = len (lst)

        for x in lst:
            if x.id == obj.id:
                if type (x) == ObjAnim:
                    # Duplicated ANIM objects are allowed
                    x.Append (self, inf, obj)
                    return
                else:
                    self.Fail (inf, "Duplicate %s identifier: '%s'" % (obj.type, obj.id))

        lst.append (obj)


    def Used (self, inf, otype, l):
        if otype == "glyph":
            tmp = ObjText ("tmp", l.GetString (), 0)
            glyph_index = self.bitmaps + self.anims
            tmp.Prepare (inf, self, glyph_index)
            tmp.MarkUsed ()
        else:
            self.Fail (inf, "unkown object type to mark used: '%s'" % otype)


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


    def ParseMenu (self, inf, level, mid, itemh, yshift):
        items = []

        while not inf.eof:
            l = inf.GetLine (inf, self)
            if l is None:
                continue

            indent = l.GetIndent ()
            if indent <= level:
                inf.PutLine (l)
                break

            t = l.GetToken ()
            if not t:
                continue

            args = []
            spar = t.find ('(')
            if spar >= 0:
                epar = t.find (')', spar + 1)
                if epar < 0:
                    self.Fail (inf, u"closing parenthesis missing in `%s'" % t)
                args = t [spar + 1:epar].split (',')
                t = t [:spar]

            act = self.ObjFind (self.actions, t)
            if act is None:
                self.Fail (inf, u"action '%s' not defined" % t)

            tid = l.GetToken ()
            if tid is None:
                self.Fail (inf, u"expected text object identifier")
            txt = self.ObjFind (self.texts, tid)
            if txt is None:
                # Try to interpret as anonymous text object declaration
                spc = self.GetSpacing (inf, tid)
                tid = "anon%d" % len (self.texts)
                text = l.GetString ()
                if len (text) == 0:
                    self.Fail (inf, "text string is missing")
                txt = ObjText (tid, text, spc)
                self.ObjAdd (inf, self.texts, txt)

            items.append ([act, args, txt])

            if l.s:
                self.Fail (inf, u"excess characters in line: `%s'" % l.s)

        self.ObjAdd (inf, self.menus, ObjMenu (mid, items, itemh, yshift))


    def ParseLayout (self, inf, level, lid, lw, lh):
        bytecode = []
        refs = []
        has_menu = False

        while not inf.eof:
            l = inf.GetLine (inf, self)
            if l is None:
                continue

            indent = l.GetIndent ()
            if indent <= level:
                inf.PutLine (l)
                break

            t = l.GetToken ()
            if not t:
                continue

            if t == "color":
                color = self.GetNum (inf, l, "color number")
                bytecode.append ([".byte", "LOP_COLOR"])
                bytecode.append ([".uleb128", str (color)])

            elif (t == "dot"):
                x = self.GetNum (inf, l, "x coordinate")
                y = self.GetNum (inf, l, "y coordinate")
                bytecode.append ([".byte", "LOP_DOT"])
                bytecode.append ([".sleb128", str (x), str (y)])

            elif (t == "line") or (t == "rect") or (t == "box"):
                x1 = self.GetNum (inf, l, "x1 coordinate")
                y1 = self.GetNum (inf, l, "y1 coordinate")
                x2 = self.GetNum (inf, l, "x2 coordinate")
                y2 = self.GetNum (inf, l, "y2 coordinate")
                bytecode.append ([".byte", "LOP_%s" % str (t).upper ()])
                bytecode.append ([".sleb128", str (x1), str (y1), str (x2), str (y2)])

            elif t == "draw":
                did = l.GetToken ()
                if (did is None):
                    self.Fail (inf, "expected layout identifier")

                lay = self.ObjFind (self.layouts, did)
                if (lay is None):
                    self.Fail (inf, "layout '%s' not defined" % did)

                x = self.GetNum (inf, l, "x coordinate")
                y = self.GetNum (inf, l, "y coordinate")
                bytecode.append ([".byte", "LOP_DRAW"])
                bytecode.append ([".sleb128", str (x), str (y)])
                bytecode.append ([".byte", "%d" % lay.index])
                refs.append (lay)

            elif t == "text":
                tid = l.GetToken ()
                if (tid is None):
                    self.Fail (inf, "expected text identifier")

                txt = self.ObjFind (self.texts, tid)
                if (txt is None):
                    self.Fail (inf, "text '%s' not defined" % tid)

                x = self.GetNum (inf, l, "x coordinate")
                y = self.GetNum (inf, l, "y coordinate")
                bytecode.append ([".byte", "LOP_TEXT"])
                bytecode.append ([".sleb128", str (x), str (y)])
                bytecode.append ([".uleb128", "_TEXT_%s-goc_texts" % txt.id])
                refs.append (txt)

            elif t == "menu":
                if has_menu:
                    self.Fail (inf, "only one menu per layout allowed")
                has_menu = True

                mid = l.GetToken ()
                if mid is None:
                    self.Fail (inf, "menu ID expected")
                menu = self.ObjFind (self.menus, mid)
                if menu is None:
                    self.Fail (inf, "menu with identifier '%s' has not been defined" % mid)

                x1 = self.GetNum (inf, l, "x1 coordinate")
                y1 = self.GetNum (inf, l, "y1 coordinate")
                x2 = self.GetNum (inf, l, "x2 coordinate")
                y2 = self.GetNum (inf, l, "y2 coordinate")

                bytecode.append ([".byte", "LOP_MENU"])
                bytecode.append ([".sleb128", str (x1), str (y1), str (x2), str (y2)])
                bytecode.append ([".byte", menu.index])
                refs.append (menu)

            else:
                self.Fail (inf, u"unknown keyword `%s'" % t)

            if l.s:
                self.Fail (inf, u"excess characters in line: `%s'" % l.s)

        bytecode.append ([".byte", "LOP_END"])

        self.ObjAdd (inf, self.layouts, ObjLayout (lid, lw, lh, bytecode, refs))


    def GetNum (self, inf, l, desc):
        t = l.GetToken ()
        try:
            t = int (t, 0)
        except (TypeError, ValueError):
            if t is None:
                t = "none"
            self.Fail (inf, "expected %s, got %s" % (desc, t))

        return t


    def GetSpacing (self, inf, t):
        try:
            t = int (t, 0)

        except (TypeError, ValueError):
            self.Fail (inf, u"TEXT spacing must be a integer, got `%s'" % t)

        if (t < -128) or (t > 127):
            self.Fail (inf, "TEXT spacing must be in range -128..127: %d" % t);

        return t


    def AddExtern (self, sym, typ, comment):
        s = u"extern %s %s[];\t/* %s */" % (typ, sym, comment)
        self.externs.append (s.encode ("utf-8"));


    def AddMacro (self, macro, value, comment):
        if type (value) != unicode:
            value = unicode (value)
        s = u"#define %s\t%s\t/* %s */" % (macro, value, comment)
        self.macros.append (s.encode ("utf-8"))


    def MarkUsed (self, lst):
        for x in lst:
            x.MarkUsed ()


    def RemoveUnused (self, lst, glyph_index = None):
        for i in range (len (lst) - 1, -1, -1):
            x = lst [i]

            if x.used:
                continue

            if not glyph_index is None:
                glyph_index [x.index] = None

            del lst [i]


    def WriteIndexed (self, outf, lst, name):
        self.AddMacro ("GOC_%sS_COUNT" % name.upper (), len (lst),
            "length of the goc_%s_index[] array" % name)

        # Write out the index
        outf.write ("\t.globl goc_%s_index\ngoc_%s_index:\n" % (name, name))
        for x in lst:
            outf.write ("\t.short\t_%s-goc_%ss\n" % (x.CName (), name))

        # Write out the objects
        outf.write ("\t.globl goc_%ss\ngoc_%ss:\n" % (name, name))
        for x in lst:
            x.Write (outf, self)


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
        self.generated_files.append (outfn)

        outf.write (
"""/* File auto-generated by gbc, do not modify */

#include "yagl/glbc.h"

\t.text\n
""")

        # Resolve all aliases
        for x in self.aliases:
            for y in self.bitmaps + self.anims:
                if y.id == x.refid:
                    x.Resolve (y)
                    break

            if not hasattr (x, 'ref'):
                self.Fail (outfn, "Failed to resolve alias '%s' to '%s'" % (x.id, x.refid))

        # When optimizing, do it in two passes
        for optimize_pass in (1, 2):
            # Clear macros and externs
            self.macros = []
            self.externs = []
            # Assign indices to all objects.
            glyph_index = [None] * 256

            # First insert single-char objects to respective positions
            free_idx = 1
            for x in self.bitmaps + self.anims + self.aliases:
                x.index = -1
                if len (x.id) == 1:
                    if opts.OutEnc != None:
                        try:
                            idx = ord (x.id.encode (opts.OutEnc)) + x.idshift
                        except UnicodeDecodeError:
                            self.Fail (outfn, "Failed to encode identifier '%s'" % x.id)
                    else:
                        if type (x) == ObjAlias:
                            continue

                        idx = free_idx
                        free_idx += 1

                    if glyph_index [idx] != None:
                        self.Fail (outfn, "Duplicate object ID: '%s'+%d" % (x.id, x.idshift))
                    x.index = idx
                    glyph_index [idx] = x

            # Now insert all other objects into free slots
            free_idx = 1 # code 0 is reserved for var references
            for x in self.bitmaps + self.anims + self.aliases:
                if x.index == -1:
                    if (opts.OutEnc == None) and (type (x) == ObjAlias):
                        continue

                    while (free_idx < len (glyph_index)) and (glyph_index [free_idx] != None):
                        free_idx += 1

                    if free_idx >= len (glyph_index):
                        self.Fail (outfn, "Too many glyph objects defined (256 maximum)")

                    x.index = free_idx
                    glyph_index [free_idx] = x
                    free_idx += 1

            # Convert text into lists of glyph indices
            for x in self.texts:
                x.Prepare (outf, self, glyph_index)

            if opts.Optimize == "":
                # No need for second pass if not optimizing
                break

            # If optimization is enabled, remove unused objects now
            if opts.Optimize.find ('l') != -1:
                self.MarkUsed (self.layouts)
            if opts.Optimize.find ('m') != -1:
                self.MarkUsed (self.menus)
            if opts.Optimize.find ('t') != -1:
                self.MarkUsed (self.texts)
            if opts.Optimize.find ('g') != -1:
                self.MarkUsed (self.bitmaps)
                self.MarkUsed (self.anims)

            # Remove all unused objects
            self.RemoveUnused (self.bitmaps, glyph_index)
            self.RemoveUnused (self.anims, glyph_index)
            self.RemoveUnused (self.aliases, glyph_index)
            self.RemoveUnused (self.texts)
            self.RemoveUnused (self.menus)
            self.RemoveUnused (self.layouts)


        # Remove last unused glyphs (save space)
        for n in range (255, -2, -1):
            if (n == -1) or (glyph_index [n] != None):
                del glyph_index [n+1:]
                break

        if len (glyph_index):
            # Write out the glyph type bitmap
            glyph_types = [0] * ((len (glyph_index) + 7) / 8)
            for n in range (len (glyph_index)):
                if type (glyph_index [n]) == ObjAnim:
                    glyph_types [n / 8] |= 1 << (n & 7);

            outf.write ("\t.globl goc_glyph_types\ngoc_glyph_types:\n")
            WriteByteArray (outf, glyph_types);

            # Finally, write the glyph index into output file
            outf.write ("\t.globl goc_glyph_index\ngoc_glyph_index:\n")
            for n in range (len (glyph_index)):
                if (glyph_index [n] == None) or \
                   (type (glyph_index [n]) is ObjVariable):
                    s = "0"
                elif type (glyph_index [n]) is ObjAlias:
                    s = "_" + glyph_index [n].ref.CName () + "-goc_glyphs"
                else:
                    s = "_" + glyph_index [n].CName () + "-goc_glyphs"
                outf.write ("\t.short\t%s\n" % s)

        # Write out the bitmaps and animations
        outf.write ("\t.globl goc_glyphs\ngoc_glyphs:\n")
        for x in self.bitmaps + self.anims + self.aliases:
            x.Write (outf, self)

        # Write out the variables
        for x in self.variables:
            x.Write (self)

        # Write out the actions
        for x in self.actions:
            x.Write (self)

        # Write out the TEXT objects
        outf.write ("\t.globl goc_texts\ngoc_texts:\n")
        for x in self.texts:
            x.Write (outf, self)

        # Write the menu index and MENU objects into output file
        self.WriteIndexed (outf, self.menus, "menu")

        # Write the layout index and LAYOUT objects into output file
        self.WriteIndexed (outf, self.layouts, "layout")

        # Don't leave IP at odd address, this leads to strange linker errors
        outf.write (".align 2\n")
        outf.close ()

        # Generate the C header file
        if hdrfn != "":
            hdrf = file (hdrfn, "w")
            self.generated_files.append (hdrfn)

            hdrf.write (
"""/* File auto-generated by gbc, do not modify */
#ifndef __%(name)s_h__
#define __%(name)s_h__

#include <stdint.h>
""" % fncomp)

            hdrf.write ("""
/* Glyph indexes in the goc_glyph_index[] table */
""")
            hdrf.write ("\n".join (self.macros) + "\n")
            hdrf.write ("""
/* Definitions of tables and graphics objects */
""")
            hdrf.write ("\n".join (self.externs) + "\n")

            if len (self.layouts):
                hdrf.write ("""
#define IMPLEMENT_MENUS \\
	g_menu_t g_menus [GOC_MENUS_COUNT]
#define IMPLEMENT_LAYOUTS \\
	g_layout_t g_layouts [GOC_LAYOUTS_COUNT]
""")

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
parser.add_option("-E", "--output-encoding", dest="OutEnc", default=None,
                  help="set encoding for indexing glyph bitmaps", metavar="ENC")
parser.add_option("-O", "--optimize", dest="Optimize", default="",
                  help="optimize keeping all layouts (l), menus (m), texts (t), glyphs (g)", metavar="LIST")

(opts, args) = parser.parse_args ()

if len (args) == 0:
    parser.print_help ()
    sys.exit (-1)

if not opts.IncludePath:
    opts.IncludePath = []

c = Compiler ()
for fn in args:
    c.Parse (fn)
c.Write (opts)
