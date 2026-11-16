"""VU micro-mode disassembler (PS2 VU0/VU1 microcode).

The game's microcode is stored as VIF packets: VIF codes (NOP, MPG,
UNPACK, MSCAL, ...) with the program pairs as MPG data. This tool walks
the VIF stream, builds the micro memory image the MPGs load, and
disassembles every loaded pair:

    address   upper word / lower word   upper (FMAC) | lower

Addresses are byte addresses in micro memory (MSCAL/vcallms/b targets
all use them; divide by 8 for the pair index the hardware counts in).
Branch targets get labels; entry points (MSCAL in the stream, vcallms or
MSCAL found elsewhere, --entry) get entry_ labels. A flow pass from the
entries marks unreached pairs, follows bal/jr (resolving jr through
constants put in vi registers by iaddiu/iaddi/bal), and lists the XGKICK
sites and the E-bit (end) points.

Operand syntax is Sony's (dvp-as / VCL): vf01x is field x of vf01, ACC,
Q, I, P, R are the special registers; lq/sq/ilw/isw offsets are in
quadwords. Anything the decoder does not know is printed as
`??? (unknown ...)` with the raw word, never guessed.

Usage:
    python port/tools/vu_disasm.py --game            # both blocks of the
        main executable into build/port/vu/ (needs build/SLUS_204.17.elf)
    python port/tools/vu_disasm.py FILE --vu 1 [--offset N] [--size N]
        [--raw] [--entry 0x340[:name]] ... [-o out.txt]
    python port/tools/vu_disasm.py --selftest        # checks the FMAC and
        lower-op decoders against the VU0 macro-mode instructions that the
        asm/ listings (rabbitizer) decode

The output is the game's code: it goes to build/ (gitignored) only.
"""

import argparse
import os
import re
import struct
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

MICRO_SIZE = {0: 0x1000, 1: 0x4000}   # micro memory bytes
DATA_SIZE = {0: 0x1000, 1: 0x4000}    # data memory bytes (VU addresses in qwords: /16)

# --------------------------------------------------------------------------
# field helpers


def sx(v, bits):
    m = 1 << (bits - 1)
    return (v & (m - 1)) - (v & m)


def dest(w):
    return "".join(c for c, b in zip("xyzw", (24, 23, 22, 21)) if w >> b & 1)


def vf(n):
    return "vf%02d" % n


def vi(n):
    # micro-mode lower fields are 5 bits but only vi00-vi15 exist there
    return "vi%02d" % n if n < 16 else "vi%02d?" % n


def ff(n, f):
    return vf(n) + "xyzw"[f]


def fmt_imm(v):
    return ("-0x%x" % -v) if v < 0 else ("0x%x" % v)


def f32(w):
    return struct.unpack("<f", struct.pack("<I", w & 0xFFFFFFFF))[0]


# --------------------------------------------------------------------------
# upper (FMAC) word

_BC_OPS = ["add", "sub", "madd", "msub", "max", "mini", "mul"]          # upper[0x00-0x1B]
_UP1 = {0x1C: ("mulq", "Q"), 0x1D: ("maxi", "I"), 0x1E: ("muli", "I"), 0x1F: ("minii", "I"),
        0x20: ("addq", "Q"), 0x21: ("maddq", "Q"), 0x22: ("addi", "I"), 0x23: ("maddi", "I"),
        0x24: ("subq", "Q"), 0x25: ("msubq", "Q"), 0x26: ("subi", "I"), 0x27: ("msubi", "I")}
_UP1_FULL = {0x28: "add", 0x29: "madd", 0x2A: "mul", 0x2B: "max", 0x2C: "sub", 0x2D: "msub",
             0x2E: "opmsub", 0x2F: "mini"}
# second table: index = bits 10:6 << 2 | bits 1:0, used when bits 5:2 == 1111
_ACC_BC_OPS = ["adda", "suba", "madda", "msuba"]                            # [0x00-0x0F]
_UP2_QI = {28: ("mulaq", "Q"), 30: ("mulai", "I"), 32: ("addaq", "Q"), 33: ("maddaq", "Q"),
           34: ("addai", "I"), 35: ("maddai", "I"), 36: ("subaq", "Q"), 37: ("msubaq", "Q"),
           38: ("subai", "I"), 39: ("msubai", "I")}
_UP2_FULL = {40: "adda", 41: "madda", 42: "mula", 44: "suba", 45: "msuba", 46: "opmula"}
_CONV = ["0", "4", "12", "15"]

FLAG_BITS = [(31, "I"), (30, "E"), (29, "M"), (28, "D"), (27, "T")]


def decode_upper(w):
    """-> (text, info). info: flags (set of 'I','E','M','D','T'), unknown (bool), notes."""
    info = {"flags": {n for b, n in FLAG_BITS if w >> b & 1}, "unknown": False, "notes": [],
            "writes_acc": False, "fd": None}
    d = dest(w)
    ft, fs, fd = (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31
    op = w & 0x3F
    bc = w & 3
    text = None
    if (w >> 25) & 3:
        info["notes"].append("upper bits 26:25 = %d (expected 0)" % ((w >> 25) & 3))
    if op < 0x1C:
        name = _BC_OPS[op >> 2] + "xyzw"[bc]
        text = "%s.%s %s, %s, %s" % (name, d, vf(fd), vf(fs), ff(ft, bc))
        info["fd"] = fd
    elif op in _UP1:
        name, reg = _UP1[op]
        text = "%s.%s %s, %s, %s" % (name, d, vf(fd), vf(fs), reg)
        info["fd"] = fd
    elif op in _UP1_FULL:
        text = "%s.%s %s, %s, %s" % (_UP1_FULL[op], d, vf(fd), vf(fs), vf(ft))
        info["fd"] = fd
    elif op >= 0x3C:
        i2 = ((w >> 6) & 31) << 2 | bc
        if i2 < 16:
            name = _ACC_BC_OPS[i2 >> 2] + "xyzw"[bc]
            text = "%s.%s ACC, %s, %s" % (name, d, vf(fs), ff(ft, bc))
            info["writes_acc"] = True
        elif i2 < 20:
            text = "itof%s.%s %s, %s" % (_CONV[bc], d, vf(ft), vf(fs))
            info["fd"] = ft
        elif i2 < 24:
            text = "ftoi%s.%s %s, %s" % (_CONV[bc], d, vf(ft), vf(fs))
            info["fd"] = ft
        elif i2 < 28:
            text = "mula%s.%s ACC, %s, %s" % ("xyzw"[bc], d, vf(fs), ff(ft, bc))
            info["writes_acc"] = True
        elif i2 in _UP2_QI:
            name, reg = _UP2_QI[i2]
            text = "%s.%s ACC, %s, %s" % (name, d, vf(fs), reg)
            info["writes_acc"] = True
        elif i2 == 29:
            text = "abs.%s %s, %s" % (d, vf(ft), vf(fs))
            info["fd"] = ft
        elif i2 == 31:
            text = "clipw.%s %s, %s" % (d, vf(fs), ff(ft, 3))
            if d != "xyz":
                info["notes"].append("clip with dest .%s (normally .xyz)" % d)
        elif i2 in _UP2_FULL:
            text = "%s.%s ACC, %s, %s" % (_UP2_FULL[i2], d, vf(fs), vf(ft))
            info["writes_acc"] = True
        elif i2 == 47:
            text = "nop"
            if w & 0x07FFFC00 and (w & 0x07FFFFFF) != 0x2FF:
                info["notes"].append("nop with nonzero fields")
    if text is None:
        info["unknown"] = True
        text = "??? (unknown upper op 0x%02x)" % op
    return text, info


# --------------------------------------------------------------------------
# lower word

_LOW_LS = {0x00: "lq", 0x01: "sq", 0x04: "ilw", 0x05: "isw"}
_LOW_BR2 = {0x28: "ibeq", 0x29: "ibne"}
_LOW_BR1 = {0x2C: "ibltz", 0x2D: "ibgtz", 0x2E: "iblez", 0x2F: "ibgez"}
_LOW_FC = {0x10: "fceq", 0x11: "fcset", 0x12: "fcand", 0x13: "fcor"}
_LOW_FS = {0x14: "fseq", 0x15: "fsset", 0x16: "fsand", 0x17: "fsor"}
_LOW_FM = {0x18: "fmeq", 0x1A: "fmand", 0x1B: "fmor"}
_EFU_P_FS = {0x70: "esadd", 0x71: "ersadd", 0x72: "eleng", 0x73: "erleng",
             0x74: "eatanxy", 0x75: "eatanxz", 0x76: "esum"}
_EFU_P_FSF = {0x78: "esqrt", 0x79: "ersqrt", 0x7A: "ercpr", 0x7C: "esin", 0x7D: "eatan", 0x7E: "eexp"}


def decode_lower(w, pc, msize):
    """-> (text, info). info: kind ('branch','call','jump','calli','xgkick',None),
    target (byte address or None), cond (bool), reg writes for const tracking."""
    info = {"kind": None, "target": None, "cond": False, "unknown": False, "notes": [],
            "ivi": None}
    op = w >> 25
    d = dest(w)
    it, is_, id_ = (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31
    imm11 = sx(w & 0x7FF, 11)
    imm15 = ((w >> 10) & 0x7800) | (w & 0x7FF)
    imm12 = ((w >> 10) & 0x800) | (w & 0x7FF)
    imm24 = w & 0xFFFFFF
    fsf, ftf = (w >> 21) & 3, (w >> 23) & 3
    tgt = (pc + 8 + imm11 * 8) & (msize - 1)

    def label():
        return "@%04x" % tgt  # replaced by the label later

    text = None
    if op in _LOW_LS:
        n = _LOW_LS[op]
        if op == 0x00:
            text = "lq.%s %s, %s(%s)" % (d, vf(it), fmt_imm(imm11), vi(is_))
        elif op == 0x01:
            text = "sq.%s %s, %s(%s)" % (d, vf(is_), fmt_imm(imm11), vi(it))
        else:
            text = "%s.%s %s, %s(%s)" % (n, d, vi(it), fmt_imm(imm11), vi(is_))
            if op == 0x04:
                info["ivi"] = (it, None)
            if len(d) != 1:
                info["notes"].append("%s with %d dest fields" % (n, len(d)))
    elif op in (0x08, 0x09):
        n = "iaddiu" if op == 0x08 else "isubiu"
        text = "%s %s, %s, 0x%x" % (n, vi(it), vi(is_), imm15)
        info["ivi"] = (it, ("add" if op == 0x08 else "sub", is_, imm15))
    elif op in _LOW_FC:
        n = _LOW_FC[op]
        text = ("%s 0x%06x" % (n, imm24)) if op == 0x11 else ("%s vi01, 0x%06x" % (n, imm24))
        if op != 0x11:
            info["ivi"] = (1, None)
    elif op in _LOW_FS:
        n = _LOW_FS[op]
        text = ("%s 0x%03x" % (n, imm12)) if op == 0x15 else ("%s %s, 0x%03x" % (n, vi(it), imm12))
        if op != 0x15:
            info["ivi"] = (it, None)
    elif op in _LOW_FM:
        text = "%s %s, %s" % (_LOW_FM[op], vi(it), vi(is_))
        info["ivi"] = (it, None)
    elif op == 0x1C:
        text = "fcget %s" % vi(it)
        info["ivi"] = (it, None)
    elif op == 0x20:
        text = "b %s" % label()
        info.update(kind="branch", target=tgt)
    elif op == 0x21:
        text = "bal %s, %s" % (vi(it), label())
        info.update(kind="call", target=tgt)
        info["ivi"] = (it, ("const", (pc + 16) // 8))
    elif op == 0x24:
        text = "jr %s" % vi(is_)
        info.update(kind="jump", reg=is_)
    elif op == 0x25:
        text = "jalr %s, %s" % (vi(it), vi(is_))
        info.update(kind="calli", reg=is_)
        info["ivi"] = (it, ("const", (pc + 16) // 8))
    elif op in _LOW_BR2:
        text = "%s %s, %s, %s" % (_LOW_BR2[op], vi(it), vi(is_), label())
        info.update(kind="branch", target=tgt, cond=True)
    elif op in _LOW_BR1:
        text = "%s %s, %s" % (_LOW_BR1[op], vi(is_), label())
        info.update(kind="branch", target=tgt, cond=True)
    elif op == 0x40:
        sub = w & 0x3F
        if sub in (0x30, 0x31, 0x34, 0x35):
            n = {0x30: "iadd", 0x31: "isub", 0x34: "iand", 0x35: "ior"}[sub]
            text = "%s %s, %s, %s" % (n, vi(id_), vi(is_), vi(it))
            info["ivi"] = (id_, (n, is_, it))
        elif sub == 0x32:
            imm5 = sx((w >> 6) & 31, 5)
            text = "iaddi %s, %s, %s" % (vi(it), vi(is_), fmt_imm(imm5))
            info["ivi"] = (it, ("add", is_, imm5))
        elif sub >= 0x3C:
            i2 = ((w >> 6) & 31) << 2 | (w & 3)
            if i2 == 0x30:
                text = "move.%s %s, %s" % (d, vf(it), vf(is_))
                if not d and it == 0 and is_ == 0:
                    text = "nop"
            elif i2 == 0x31:
                text = "mr32.%s %s, %s" % (d, vf(it), vf(is_))
            elif i2 == 0x34:
                text = "lqi.%s %s, (%s++)" % (d, vf(it), vi(is_))
                info["ivi"] = (is_, None)
            elif i2 == 0x35:
                text = "sqi.%s %s, (%s++)" % (d, vf(is_), vi(it))
                info["ivi"] = (it, None)
            elif i2 == 0x36:
                text = "lqd.%s %s, (--%s)" % (d, vf(it), vi(is_))
                info["ivi"] = (is_, None)
            elif i2 == 0x37:
                text = "sqd.%s %s, (--%s)" % (d, vf(is_), vi(it))
                info["ivi"] = (it, None)
            elif i2 == 0x38:
                text = "div Q, %s, %s" % (ff(is_, fsf), ff(it, ftf))
            elif i2 == 0x39:
                text = "sqrt Q, %s" % ff(it, ftf)
            elif i2 == 0x3A:
                text = "rsqrt Q, %s, %s" % (ff(is_, fsf), ff(it, ftf))
            elif i2 == 0x3B:
                text = "waitq"
            elif i2 == 0x3C:
                text = "mtir %s, %s" % (vi(it), ff(is_, fsf))
                info["ivi"] = (it, None)
            elif i2 == 0x3D:
                text = "mfir.%s %s, %s" % (d, vf(it), vi(is_))
            elif i2 == 0x3E:
                text = "ilwr.%s %s, (%s)" % (d, vi(it), vi(is_))
                info["ivi"] = (it, None)
            elif i2 == 0x3F:
                text = "iswr.%s %s, (%s)" % (d, vi(it), vi(is_))
            elif i2 == 0x40:
                text = "rnext.%s %s, R" % (d, vf(it))
            elif i2 == 0x41:
                text = "rget.%s %s, R" % (d, vf(it))
            elif i2 == 0x42:
                text = "rinit R, %s" % ff(is_, fsf)
            elif i2 == 0x43:
                text = "rxor R, %s" % ff(is_, fsf)
            elif i2 == 0x64:
                text = "mfp.%s %s, P" % (d, vf(it))
            elif i2 == 0x68:
                text = "xtop %s" % vi(it)
                info["ivi"] = (it, None)
            elif i2 == 0x69:
                text = "xitop %s" % vi(it)
                info["ivi"] = (it, None)
            elif i2 == 0x6C:
                text = "xgkick %s" % vi(is_)
                info["kind"] = "xgkick"
                info["reg"] = is_
            elif i2 in _EFU_P_FS:
                text = "%s P, %s" % (_EFU_P_FS[i2], vf(is_))
            elif i2 in _EFU_P_FSF:
                text = "%s P, %s" % (_EFU_P_FSF[i2], ff(is_, fsf))
            elif i2 == 0x7B:
                text = "waitp"
            if text is None:
                text = "??? (unknown lower 0x40 sub-op 0x%02x)" % i2
                info["unknown"] = True
    if text is None:
        text = "??? (unknown lower op 0x%02x)" % op
        info["unknown"] = True
    return text, info


# --------------------------------------------------------------------------
# VIF stream


VIF_SIMPLE = {0x00: "NOP", 0x01: "STCYCL", 0x02: "OFFSET", 0x03: "BASE", 0x04: "ITOP",
              0x05: "STMOD", 0x06: "MSKPATH3", 0x07: "MARK", 0x10: "FLUSHE", 0x11: "FLUSH",
              0x13: "FLUSHA", 0x14: "MSCAL", 0x15: "MSCALF", 0x17: "MSCNT"}
UNPACK_FMT = {0x0: "S-32", 0x1: "S-16", 0x2: "S-8", 0x4: "V2-32", 0x5: "V2-16", 0x6: "V2-8",
              0x8: "V3-32", 0x9: "V3-16", 0xA: "V3-8", 0xC: "V4-32", 0xD: "V4-16", 0xE: "V4-8",
              0xF: "V4-5"}


def unpack_words(vnvl, num):
    vn, vl = vnvl >> 2, vnvl & 3
    if vnvl == 0xF:
        bits = 16
    else:
        bits = (vn + 1) * (32 >> vl)
    return (num * bits + 31) // 32


def parse_vif(data, vu, base_addr=0):
    """Walks a VIF packet. -> (events, mem) where mem maps micro byte address to
    (upper, lower, stream offset) and events lists (offset, text, kind, value)."""
    events = []
    mem = {}
    pos = 0
    msize = MICRO_SIZE[vu]
    wl, cl = 1, 1
    while pos + 4 <= len(data):
        code = struct.unpack_from("<I", data, pos)[0]
        cmd = (code >> 24) & 0x7F
        irq = " (i)" if code >> 31 else ""
        num = (code >> 16) & 0xFF
        imm = code & 0xFFFF
        at = pos
        pos += 4
        if cmd in VIF_SIMPLE:
            n = VIF_SIMPLE[cmd]
            if cmd == 0x01:
                wl, cl = (imm >> 8) & 0xFF, imm & 0xFF
                events.append((at, "STCYCL wl=%d cl=%d%s" % (wl, cl, irq), "vif", None))
            elif cmd in (0x14, 0x15):
                events.append((at, "%s 0x%04x (pair %d)%s" % (n, imm * 8, imm, irq), "mscal", imm * 8))
            elif cmd in (0x00, 0x10, 0x11, 0x13, 0x17):
                events.append((at, n + irq, "vif", None))
            else:
                events.append((at, "%s 0x%x%s" % (n, imm, irq), "vif", None))
        elif cmd == 0x20:
            events.append((at, "STMASK 0x%08x" % struct.unpack_from("<I", data, pos)[0], "vif", None))
            pos += 4
        elif cmd in (0x30, 0x31):
            vals = struct.unpack_from("<4I", data, pos)
            events.append((at, "%s %s" % ("STROW" if cmd == 0x30 else "STCOL",
                                         " ".join("%08x" % v for v in vals)), "vif", None))
            pos += 16
        elif cmd == 0x4A:
            n = num or 256
            addr = imm * 8
            if pos % 8:
                events.append((at, "MPG data not 64-bit aligned (offset 0x%x)" % pos, "warn", None))
            events.append((at, "MPG %d pairs to 0x%04x-0x%04x" % (n, addr, addr + n * 8 - 8), "mpg", (addr, n)))
            for i in range(n):
                lo, up = struct.unpack_from("<II", data, pos + i * 8)
                mem[(addr + i * 8) & (msize - 1)] = (up, lo, base_addr + pos + i * 8)
            pos += n * 8
        elif cmd in (0x50, 0x51):
            n = imm or 0x10000
            pos = (pos + 15) & ~15
            events.append((at, "%s %d qwords" % ("DIRECT" if cmd == 0x50 else "DIRECTHL", n), "vif", None))
            pos += n * 16
        elif cmd >= 0x60:
            vnvl = cmd & 0xF
            m = (cmd >> 4) & 1
            n = num or 256
            addr = code & 0x3FF
            usn = (code >> 14) & 1
            flg = (code >> 15) & 1
            fmt = UNPACK_FMT.get(vnvl, "?")
            if wl > cl:
                # filling write: only cl of every wl writes read data
                nd = (n // wl) * cl + min(n % wl, cl)
            else:
                nd = n
            words = unpack_words(vnvl, nd)
            raw = data[pos:pos + words * 4]
            events.append((at, "UNPACK %s%s num=%d to VU 0x%03x (qw)%s%s%s, %d data words" %
                           (fmt, " masked" if m else "", n, addr, " unsigned" if usn else "",
                            " +TOPS" if flg else "", irq, words), "unpack", (vnvl, addr, raw)))
            if fmt == "V4-32":
                for i in range(0, len(raw) // 16):
                    q = struct.unpack_from("<4I", raw, i * 16)
                    events.append((at, "    qw %d: %s  (%s)" % (
                        addr + i, " ".join("%08x" % v for v in q),
                        ", ".join("%g" % f32(v) for v in q)), "data", None))
            pos += words * 4
        else:
            events.append((at, "unknown VIF code 0x%08x, stopping" % code, "warn", None))
            break
    return events, mem


# --------------------------------------------------------------------------
# flow analysis


class Program:
    def __init__(self, mem, vu, entries, mscnt=False, final=()):
        self.mscnt = mscnt
        self.final = set(final)
        self.conts = {}                  # MSCNT continuation -> E-bit pcs
        self.mem = mem
        self.vu = vu
        self.msize = MICRO_SIZE[vu]
        self.entries = dict(entries)  # addr -> name
        self.dec = {}
        for a, (up, lo, _) in mem.items():
            ut, ui = decode_upper(up)
            if "I" in ui["flags"]:
                lt, li = "loi %s" % repr_float(lo), {"kind": None, "target": None, "cond": False,
                                                    "unknown": False, "notes": [], "ivi": None}
            else:
                lt, li = decode_lower(lo, a, self.msize)
            self.dec[a] = (ut, ui, lt, li)
        self.reached = set()
        self.calls = defaultdict(set)    # sub addr -> caller pcs
        self.jumps = {}                  # pc -> set of resolved targets (or None)
        self.branch_targets = defaultdict(set)
        self.xgkicks = []
        self.ends = set()
        self.warnings = []
        self.flow()

    def flow(self):
        # state: tuple of sorted (vi, const) for vi registers with a known value
        self.call_sites = set()
        work = [(a, ()) for a in sorted(self.entries)]
        seen = set()
        per_pc = defaultdict(int)
        while True:
            self._run(work, seen, per_pc)
            # returns the callee's jr did not resolve: continue with no state
            work = [(a, ()) for a in sorted(self.call_sites - self.reached)]
            if not work:
                break

    def _run(self, work, seen, per_pc):
        while work:
            pc, st = work.pop()
            if (pc, st) in seen or per_pc[pc] > 64:
                continue
            seen.add((pc, st))
            per_pc[pc] += 1
            for succ, nst in self.step(pc, dict(st)):
                work.append((succ, tuple(sorted(nst.items()))))

    def apply(self, pc, s):
        """apply pc's vi writes to state dict s (in place)."""
        if pc not in self.dec:
            return
        li = self.dec[pc][3]
        w = li.get("ivi")
        if not w:
            return
        reg, how = w
        if reg == 0:
            return
        val = None
        if how is not None:
            kind = how[0]
            if kind == "const":
                val = how[1]
            elif kind in ("add", "sub"):
                # only constants built from vi00 (code addresses, sizes):
                # following loop counters would make the pass unroll loops
                src = 0 if how[1] == 0 else None
                if src is not None:
                    val = (src + how[2]) & 0xFFFF if kind == "add" else (src - how[2]) & 0xFFFF
            elif kind in ("iadd", "isub", "iand", "ior"):
                # a register copy (iadd viX, viY, vi00) keeps a known value
                a = 0 if how[1] == 0 else s.get(how[1])
                b = 0 if how[2] == 0 else s.get(how[2])
                if how[1] != 0 and how[2] != 0:
                    a = None
                if a is not None and b is not None:
                    val = {"iadd": a + b, "isub": a - b, "iand": a & b, "ior": a | b}[kind] & 0xFFFF
        if val is None:
            s.pop(reg, None)
        else:
            s[reg] = val

    def step(self, pc, s):
        m = self.msize
        if pc not in self.dec:
            self.warnings.append("flow reaches 0x%04x, which no MPG loaded" % pc)
            return []
        self.reached.add(pc)
        ut, ui, lt, li = self.dec[pc]
        nxt = (pc + 8) & (m - 1)
        kind = li["kind"]
        if kind == "xgkick":
            self.xgkicks.append(pc) if pc not in self.xgkicks else None
        if "E" in ui["flags"]:
            self.ends.add(pc)
            self.apply(pc, s)
            if nxt in self.dec:
                self.reached.add(nxt)
                if self.dec[nxt][3]["kind"] in ("branch", "call", "jump", "calli"):
                    self.warnings.append("branch in the E-bit delay slot at 0x%04x" % nxt)
                if "E" in self.dec[nxt][1]["flags"]:
                    self.warnings.append("E bit in the E-bit delay slot at 0x%04x" % nxt)
                if self.dec[nxt][3]["kind"] == "xgkick" and nxt not in self.xgkicks:
                    self.xgkicks.append(nxt)
                self.apply(nxt, s)
            kicks = kind == "xgkick" or (nxt in self.dec and self.dec[nxt][3]["kind"] == "xgkick")
            if self.mscnt and pc not in self.final and not kicks:
                # VIF MSCNT starts the next program where this one stopped
                # (the pair after the E-bit delay slot); VU registers persist.
                # An end that XGKICKs (or is listed in final) is taken as the
                # end of the whole program: nothing continues it
                c = (pc + 16) & (m - 1)
                self.conts.setdefault(c, set()).add(pc)
                return [(c, s)]
            return []
        if kind in ("branch", "call", "jump", "calli"):
            return self._branch(pc, s, nxt)
        self.apply(pc, s)
        return [(nxt, s)]

    def _branch(self, pc, s, ds, nested=False):
        """successors of the branch at pc whose delay slot is the pair at ds
        (pc + 8, or the first branch's target when pc itself sits in a delay
        slot: the hardware then runs one pair at the first target as the
        second branch's delay slot, then goes to the second target)."""
        m = self.msize
        li = self.dec[pc][3]
        kind = li["kind"]
        # the register read happens before the delay slot executes
        jr_val = s.get(li.get("reg")) if kind in ("jump", "calli") else None
        if kind in ("jump", "calli") and li.get("reg") == 0:
            jr_val = 0
        self.apply(pc, s)
        after = (ds + 8) & (m - 1)
        dk = self.dec[ds][3]["kind"] if ds in self.dec else None
        inner = dk in ("branch", "call", "jump", "calli") and not nested
        if ds in self.dec:
            self.reached.add(ds)
            if dk in ("branch", "call", "jump", "calli"):
                self.warnings.append("branch in a delay slot at 0x%04x (followed as: one pair at the "
                                     "first target, then the second branch)" % ds)
            if dk == "xgkick" and ds not in self.xgkicks:
                self.xgkicks.append(ds)
            if "E" in self.dec[ds][1]["flags"]:
                self.warnings.append("E bit in a branch delay slot at 0x%04x" % ds)
            if not inner:
                self.apply(ds, s)
        out = []
        if kind == "branch":
            self.branch_targets[li["target"]].add(pc)
            out.append((li["target"], dict(s)))
            if li["cond"]:
                out.append((after, dict(s)))
        elif kind == "call":
            self.calls[li["target"]].add(pc)
            out.append((li["target"], dict(s)))
            self.call_sites.add(after)  # reached by the callee's jr, or by fallback
        elif kind == "jump":
            if jr_val is not None:
                t = (jr_val * 8) & (m - 1)
                self.jumps.setdefault(pc, set()).add(t)
                self.branch_targets[t].add(pc)
                out.append((t, dict(s)))
            else:
                self.jumps.setdefault(pc, set()).add(None)
        elif kind == "calli":
            if jr_val is not None:
                t = (jr_val * 8) & (m - 1)
                self.jumps.setdefault(pc, set()).add(t)
                self.calls[t].add(pc)
                out.append((t, dict(s)))
            else:
                self.jumps.setdefault(pc, set()).add(None)
            self.call_sites.add(after)
        if inner:
            res = []
            for t, st in out:
                if t in self.dec:
                    self.reached.add(t)
                res += self._branch(ds, st, t, nested=True)
            return res
        return out

    def labels(self):
        lab = {}
        for a, n in self.entries.items():
            lab[a] = n
        for a in self.calls:
            lab.setdefault(a, "sub_%04x" % a)
        for a in self.conts:
            lab.setdefault(a, "cont_%04x" % a)
        for a in self.branch_targets:
            lab.setdefault(a, "L_%04x" % a)
        # static branch targets in unreached code too
        for a, (ut, ui, lt, li) in self.dec.items():
            if li["kind"] in ("branch", "call") and li["target"] is not None:
                lab.setdefault(li["target"], ("sub_%04x" if li["kind"] == "call" else "L_%04x") % li["target"])
        return lab


def repr_float(w):
    v = f32(w)
    return "%s  (0x%08x)" % (("%.9g" % v), w)


# --------------------------------------------------------------------------
# listing


def listing(name, data, vu, entries, out, base_addr=0, symbol=None, mscnt=None, final=()):
    events, mem = parse_vif(data, vu, base_addr)
    for ofs, text, kind, val in events:
        if kind == "mscal":
            entries.setdefault(val, "entry_%04x" % val)
    if mscnt is None:
        mscnt = vu == 1
    prog = Program(mem, vu, entries, mscnt, final)
    lab = prog.labels()
    w = out.write
    w("; %s: VU%d microcode, disassembled by port/tools/vu_disasm.py\n" % (name, vu))
    w("; game code - build/ only, never commit\n")
    if symbol:
        w("; source: %s\n" % symbol)
    w("; %d bytes of VIF stream, %d pairs loaded into micro memory (%d bytes of %d)\n" %
      (len(data), len(mem), len(mem) * 8, MICRO_SIZE[vu]))
    w("; addresses are micro-memory byte addresses (pair index = address / 8)\n;\n")
    w("; ---- VIF stream ----\n")
    for ofs, text, kind, val in events:
        if kind == "mpg" or kind not in ("data",):
            w(";  +0x%04x  %s\n" % (ofs, text))
        else:
            w(";           %s\n" % text)
    w(";\n; ---- entry points ----\n")
    for a in sorted(prog.entries):
        w(";  0x%04x  %s\n" % (a, prog.entries[a]))
    w(";\n; ---- MSCNT continuations (the pair after an E-bit end's delay slot; %s) ----\n" %
      ("followed by the flow pass, except after ends that XGKICK" if mscnt else "not followed"))
    for a in sorted(prog.conts):
        w(";  0x%04x  after the end at %s\n" % (a, ", ".join("0x%04x" % c for c in sorted(prog.conts[a]))))
    w(";\n; ---- subroutines (bal/jalr targets) ----\n")
    for a in sorted(prog.calls):
        w(";  0x%04x  %s  called from %s\n" % (a, lab.get(a), ", ".join("0x%04x" % c for c in sorted(prog.calls[a]))))
    w(";\n; ---- jr/jalr ----\n")
    for a in sorted(prog.jumps):
        ts = prog.jumps[a]
        w(";  0x%04x  %s -> %s\n" % (a, prog.dec[a][2], ", ".join(
            ("unresolved (return or computed)" if t is None else "0x%04x %s" % (t, lab.get(t, ""))) for t in
            sorted(ts, key=lambda x: -1 if x is None else x))))
    w(";\n; ---- XGKICK sites ----\n")
    for a in sorted(prog.xgkicks):
        w(";  0x%04x  %s\n" % (a, prog.dec[a][2]))
    w(";\n; ---- E bit (program end; the next pair still executes) ----\n")
    for a in sorted(prog.ends):
        w(";  0x%04x\n" % a)
    unreached = sorted(a for a in mem if a not in prog.reached)
    w(";\n; ---- pairs not reached from the entries: %d ----\n" % len(unreached))
    for lo, hi in ranges(unreached):
        w(";  0x%04x-0x%04x\n" % (lo, hi))
    unknown = [a for a, d in prog.dec.items() if d[1]["unknown"] or d[3]["unknown"]]
    notes = [(a, n) for a, d in sorted(prog.dec.items()) for n in d[1]["notes"] + d[3]["notes"]]
    w(";\n; ---- decoder problems: %d unknown words, %d notes ----\n" % (len(unknown), len(notes)))
    for a in sorted(unknown):
        w(";  0x%04x  unknown encoding (%s%s)%s\n" % (
            a, "reached" if a in prog.reached else "not reached",
            "", ""))
    for a, n in notes:
        w(";  0x%04x  %s\n" % (a, n))
    for n in sorted(set(prog.warnings)):
        w(";  flow: %s\n" % n)
    w(";\n; ---- code ----\n")
    prev = None
    for a in sorted(mem):
        up, lo, src = mem[a]
        ut, ui, lt, li = prog.dec[a]
        if prev is not None and a != prev + 8:
            w("\n; (gap)\n")
        prev = a
        if a in lab:
            w("\n%s:\n" % lab[a])
        lt2 = re.sub(r"@([0-9a-f]{4})", lambda m: lab.get(int(m.group(1), 16), "0x" + m.group(1)), lt)
        flags = "".join(sorted(ui["flags"], key="IEMDT".index))
        mark = " " if a in prog.reached else "-"
        line = "  %04x%s %08x %08x  %-5s %-40s %s" % (a, mark, up, lo, ("[" + flags + "]") if flags else "",
                                                     ut, lt2)
        extra = []
        if li["kind"] in ("branch", "call") and li["target"] is not None:
            extra.append("-> 0x%04x" % li["target"])
        if a in prog.jumps:
            extra.append("-> " + ", ".join("?" if t is None else "0x%04x" % t for t in
                                            sorted(prog.jumps[a], key=lambda x: -1 if x is None else x)))
        for n in ui["notes"] + li["notes"]:
            extra.append("NOTE: " + n)
        if ui["unknown"] or li["unknown"]:
            extra.append("UNKNOWN ENCODING")
        if extra:
            line = "%-110s ; %s" % (line, "; ".join(extra))
        w(line.rstrip() + "\n")
        if "E" in ui["flags"]:
            w("  ; ---- end (after the next pair)\n")
    return prog


def ranges(xs):
    out = []
    for a in xs:
        if out and a == out[-1][1] + 8:
            out[-1][1] = a
        else:
            out.append([a, a])
    return out


# --------------------------------------------------------------------------
# the game: symbols, entry points


def load_elf_blocks():
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    from elf32 import Elf
    elf = Elf(os.path.join(ROOT, "build", "SLUS_204.17.elf"))
    syms = elf.defined()
    blocks = []
    pairs = [("vuMicroCode0", "vuMicroCode1", 0), ("vuMicroCode1", "vuMicroCode1End", 1)]
    for a, b, vu in pairs:
        start, end = syms[a]["value"], syms[b]["value"]
        for s in elf.alloc_sections():
            if s["addr"] <= start < s["addr"] + s["size"]:
                d = elf.data(s)[start - s["addr"]:end - s["addr"]]
                blocks.append((a, vu, d, start))
                break
    return blocks


def scan_entries():
    """vcallms targets in asm/ (VU0) and MSCAL codes in src/ and asm/ (VU1)."""
    vu0 = defaultdict(set)
    vu1 = defaultdict(set)
    fn_re = re.compile(r"^glabel (\w+)")
    for dirpath, _, files in os.walk(os.path.join(ROOT, "asm")):
        for f in files:
            if not f.endswith(".s"):
                continue
            fn = None
            for line in open(os.path.join(dirpath, f), encoding="utf-8", errors="replace"):
                m = fn_re.match(line)
                if m:
                    fn = m.group(1)
                m = re.search(r"\*/\s+vcallms\s+(0x[0-9A-Fa-f]+|\d+)", line)
                if m:
                    vu0[int(m.group(1), 0)].add(fn)
                m = re.search(r"\(0x(1[45]00[0-9A-Fa-f]{4}) [>&]", line)
                if m and line.find(">> 16") >= 0:
                    vu1[(int(m.group(1), 16) & 0xFFFF) * 8].add(fn)
    for dirpath, _, files in os.walk(os.path.join(ROOT, "src")):
        for f in files:
            if not f.endswith(".c"):
                continue
            txt = open(os.path.join(dirpath, f), encoding="utf-8", errors="replace").read()
            for m in re.finditer(r"\b0x1[45]00([0-9A-Fa-f]{4})\b(?!ULL|LL)", txt):
                if not re.search(r"0x1[45]00[0-9A-Fa-f]{4}[0-9A-Fa-f]", txt[m.start():m.end() + 1]):
                    vu1[int(m.group(1), 16) * 8].add(f)
            for m in re.finditer(r"VIF_MSCAL\((\d+|0x[0-9A-Fa-f]+)\)", txt):
                vu1[int(m.group(1), 0) * 8].add(f)
    return vu0, vu1


# MSCALs whose address the code computes, so the scan can't see them:
# GphMeshPacketUnlitVu1 sends MSCAL 0x1A + 2 * (mode & 3) and
# GphMeshPacketLitVu1 MSCAL 0x22 + 2 * (mode & 3) (asm/main/main/0578B4.s)
COMPUTED_VU1 = {0xE0: "GphMeshPacketUnlitVu1 (mode 1)", 0xF0: "GphMeshPacketUnlitVu1 (mode 2)",
                0x100: "GphMeshPacketUnlitVu1 (mode 3)", 0x120: "GphMeshPacketLitVu1 (mode 1)",
                0x130: "GphMeshPacketLitVu1 (mode 2)", 0x140: "GphMeshPacketLitVu1 (mode 3)"}


# E-bit ends the CPU never continues with MSCNT: the init program run by the
# stream's own MSCAL 0 (the pair after it is a branch-to-self)
GAME_FINAL = {1: (0x158,)}


def run_game(outdir):
    os.makedirs(outdir, exist_ok=True)
    vu0e, vu1e = scan_entries()
    for a, who in COMPUTED_VU1.items():
        vu1e[a].add(who)
    for name, vu, data, start in load_elf_blocks():
        found = vu0e if vu == 0 else vu1e
        entries = {a: "entry_%04x" % a for a in found}
        path = os.path.join(outdir, "%s.txt" % name)
        with open(path, "w", encoding="utf-8", newline="\n") as out:
            out.write("; entry points used by the game's code:\n")
            for a in sorted(found):
                out.write(";   0x%04x  %s\n" % (a, ", ".join(sorted(x for x in found[a] if x))))
            prog = listing(name, data, vu, entries, out, start, "build/SLUS_204.17.elf %s at 0x%08x" % (name, start),
                           final=GAME_FINAL.get(vu, ()))
        unk = sum(1 for d in prog.dec.values() if d[1]["unknown"] or d[3]["unknown"])
        print("%s: %d pairs, %d entries, %d xgkick, %d unreached, %d unknown -> %s" % (
            name, len(prog.mem), len(prog.entries), len(prog.xgkicks),
            len(prog.mem) - len(prog.reached), unk, os.path.relpath(path, ROOT)))


# --------------------------------------------------------------------------
# self test against rabbitizer's VU0 macro-mode decoding in asm/


def norm(s):
    s = s.strip().lower().replace("$", "")
    s = re.sub(r"\bvf0*(\d+)", r"vf\1", s)
    s = re.sub(r"\bvi0*(\d+)", r"vi\1", s)
    s = re.sub(r"\s+", " ", s)
    s = re.sub(r"\s*,\s*", ",", s)
    return s


def selftest():
    seen = {}
    for dirpath, _, files in os.walk(os.path.join(ROOT, "asm")):
        for f in files:
            if not f.endswith(".s"):
                continue
            for line in open(os.path.join(dirpath, f), encoding="utf-8", errors="replace"):
                m = re.search(r"/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/\s+(v\w[\w.]*)\s*(.*)$", line)
                if not m:
                    continue
                b = bytes.fromhex(m.group(1))
                wd = struct.unpack("<I", b)[0]
                if wd >> 25 != 0x25 or m.group(2).startswith("vcallms"):
                    continue
                seen.setdefault(wd, (m.group(2), m.group(3).split("#")[0].strip()))
    ok = bad = quirk = 0
    checked = set()
    for wd, (mn, ops) in sorted(seen.items()):
        body = wd & 0x01FFFFFF
        op = body & 0x3F
        i2 = ((body >> 6) & 31) << 2 | (body & 3)
        if 0x30 <= op < 0x3C or (op >= 0x3C and i2 >= 0x30):
            text, _ = decode_lower(0x80000000 | body, 0, 0x1000)
        else:
            text, _ = decode_upper(body)
        mine = norm(text)
        # Sony vs rabbitizer: rabbitizer prefixes v, writes vclipw, and
        # names the macro-mode Q/ACC/I with $
        ref = norm(mn[1:] + " " + ops)
        swapped = None
        mm = re.match(r"(adda\.\w+) acc,(\w+),(\w+)$", mine)
        if mm:
            # rabbitizer prints vadda (only) as ACC, ft, fs; fs is bits 15:11
            # for every FMAC op and ADDA commutes, so both read the same
            swapped = "%s acc,%s,%s" % (mm.group(1), mm.group(3), mm.group(2))
        checked.add(text.split()[0].split(".")[0])
        if mine == ref:
            ok += 1
        elif swapped == ref:
            ok += 1
            quirk += 1
        else:
            bad += 1
            print("MISMATCH %08x: rabbitizer '%s %s'  mine '%s'" % (wd, mn, ops, text))
    print("selftest: %d distinct macro-mode words agree (%d of them vadda, which rabbitizer "
          "prints with fs/ft swapped), %d differ" % (ok, quirk, bad))
    print("mnemonics cross-checked: %s" % " ".join(sorted(checked)))
    print("every other op (clip, the integer and flag ops, loads/stores, branches, xgkick, the "
          "E-unit ops, loi and the E/M/D/T bits) is decoded from the VU manual's encoding tables "
          "only; the checks below are hand-worked")
    # encodings checked by hand against the VU manual's tables
    cases = [
        (0x000002FF, "nop"), (0x01E821BC, "mulax.xyzw ACC, vf04, vf08x"),
        (0x01E83A0B, "maddw.xyzw vf08, vf07, vf08w"), (0x01C001FF, "clipw.xyz vf00, vf00w"),
    ]
    lcases = [
        (0x8000033C, "nop"), (0x40000029, "b @0150"), (0x100F01B3, "iaddiu vi15, vi00, 0x1b3"),
    ]
    for wd, exp in cases:
        t = decode_upper(wd)[0]
        print("%s upper %08x -> %s" % ("ok " if t == exp else "BAD", wd, t))
    for wd, exp in lcases:
        t = decode_lower(wd, 0, 0x4000)[0]
        print("%s lower %08x -> %s" % ("ok " if t == exp else "BAD", wd, t))
    return bad == 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("file", nargs="?")
    ap.add_argument("--vu", type=int, choices=(0, 1), default=1)
    ap.add_argument("--offset", type=lambda x: int(x, 0), default=0)
    ap.add_argument("--size", type=lambda x: int(x, 0))
    ap.add_argument("--raw", action="store_true", help="the file is bare pairs loaded at 0, not a VIF stream")
    ap.add_argument("--entry", action="append", default=[], help="ADDR[:NAME] (byte address)")
    ap.add_argument("--final", action="append", default=[],
                    help="ADDR of an E-bit end that no MSCNT continues (VU1)")
    ap.add_argument("-o", "--out")
    ap.add_argument("--game", action="store_true")
    ap.add_argument("--outdir", default=os.path.join(ROOT, "build", "port", "vu"))
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args()
    if a.selftest:
        sys.exit(0 if selftest() else 1)
    if a.game:
        run_game(a.outdir)
        return
    if not a.file:
        ap.error("give a file, --game or --selftest")
    data = open(a.file, "rb").read()[a.offset:]
    if a.size:
        data = data[:a.size]
    if a.raw:
        # wrap the pairs in MPG codes so one path handles both
        out = b""
        off = 0
        while off < len(data) // 8 * 8:
            k = min(256, (len(data) - off) // 8)
            out += struct.pack("<II", 0, 0x4A000000 | ((k & 0xFF) << 16) | (off // 8)) + data[off:off + k * 8]
            off += k * 8
        data = out
    entries = {}
    for e in a.entry:
        addr, _, name = e.partition(":")
        entries[int(addr, 0)] = name or "entry_%04x" % int(addr, 0)
    out = open(a.out, "w", encoding="utf-8", newline="\n") if a.out else sys.stdout
    listing(os.path.basename(a.file), data, a.vu, entries, out, final=[int(x, 0) for x in a.final])


if __name__ == "__main__":
    main()
