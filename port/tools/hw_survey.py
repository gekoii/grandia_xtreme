#!/usr/bin/env python3
"""Survey the game's accesses to PS2 hardware and fixed addresses, for the
Windows port (port/docs/hardware.md).

On Windows every literal PS2 address is unmapped: the DMA, GIF, VIF, IPU,
INTC, timer and SIO registers (0x1000xxxx), VU memory (0x1100xxxx), the GS
privileged registers (0x1200xxxx), the scratchpad (0x70000000), the
uncached and uncached-accelerated aliases of main RAM (addr | 0x20000000,
addr | 0x30000000) and the fixed RAM areas of config/memory_map.ld. This
lists every place the code the port compiles (src/, include/, port/game/)
uses one, and what each needs from the port.

    python port/tools/hw_survey.py              # summary
    python port/tools/hw_survey.py --sites      # TSV, one line per site
    python port/tools/hw_survey.py --functions  # TSV, one line per function
    python port/tools/hw_survey.py --values     # literals in those ranges taken as values (to review)
    python port/tools/hw_survey.py --asm        # the same addresses in functions still in assembly
    python port/tools/hw_survey.py --replace    # functions that need port C replacements, and why

Site columns: file:line, function, region, address, register, access,
kind, mechanism, sdk, note. access is R, W, RW (read-modify-write) or ptr
(the address is taken as a pointer or passed on; the accesses happen
elsewhere). A read in a while/for condition is noted "poll". kind says how
the address appears:

  cast     a literal cast to a pointer
  macro    through a #define (the definitions are listed with access "def")
  arg      an uncast scratchpad address passed to a function
  value    an uncast scratchpad address stored or compared
  table    a register address in a data table
  pointer  through a local pointer variable set from a register address
  channel  a DMA channel register through a sceDmaChan pointer
  alias    | 0x20000000 / | 0x30000000 arithmetic
  range    a scratchpad range check or offset (DMA addresses)
  mask     & 0x0FFFFFFF and the like: an address masked for DMA or alignment
  linker   a name config/memory_map.ld or build/c_syms.ld fixes

mechanism is what the port needs:

  buffer   memory mapped at the same address is enough (port/src/hwmem.c)
  active   a write must start work, or a read must reflect the state of
           work the platform does (DMA start or busy poll, FIFO, GS CSR,
           IPU, INTC); a buffer alone doesn't do it
  alias    uncached-alias arithmetic on a port pointer
  phys     address masking/flagging for DMA (works while port pointers
           stay below 0x08000000; the platform's DMA decodes SPR addresses)
  load     a load area of the overlays or the disc's programs (the program
           loader and the R5900 interpreter, not memory)
  heap     a fixed RAM range the game manages itself (heap, IOP block)

The heuristics are lexical (a small C tokenizer; macros are followed within
a file and from include/); --values lists the literals they set aside as
plain values (DMA tags, colours, flags) so that can be checked.
"""

import argparse
import glob
import os
import re
import sys
from collections import Counter, defaultdict

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))

# ---- the EE's address map ------------------------------------------------

DMA_CHANNELS = {
    0x10008000: "D0", 0x10009000: "D1", 0x1000A000: "D2", 0x1000B000: "D3",
    0x1000B400: "D4", 0x1000C000: "D5", 0x1000C400: "D6", 0x1000C800: "D7",
    0x1000D000: "D8", 0x1000D400: "D9",
}
DMA_CHANNEL_DESC = {"D0": "VIF0", "D1": "VIF1", "D2": "GIF", "D3": "fromIPU", "D4": "toIPU",
                    "D5": "SIF0", "D6": "SIF1", "D7": "SIF2", "D8": "fromSPR", "D9": "toSPR"}
DMA_REGS = {0x00: "CHCR", 0x10: "MADR", 0x20: "QWC", 0x30: "TADR", 0x40: "ASR0", 0x50: "ASR1", 0x80: "SADR"}

REGS = {
    0x10000000: "T0_COUNT", 0x10000010: "T0_MODE", 0x10000020: "T0_COMP", 0x10000030: "T0_HOLD",
    0x10000800: "T1_COUNT", 0x10000810: "T1_MODE", 0x10000820: "T1_COMP", 0x10000830: "T1_HOLD",
    0x10001000: "T2_COUNT", 0x10001010: "T2_MODE", 0x10001020: "T2_COMP",
    0x10001800: "T3_COUNT", 0x10001810: "T3_MODE", 0x10001820: "T3_COMP",
    0x10002000: "IPU_CMD", 0x10002010: "IPU_CTRL", 0x10002020: "IPU_BP", 0x10002030: "IPU_TOP",
    0x10003000: "GIF_CTRL", 0x10003010: "GIF_MODE", 0x10003020: "GIF_STAT",
    0x10003040: "GIF_TAG0", 0x10003050: "GIF_TAG1", 0x10003060: "GIF_TAG2", 0x10003070: "GIF_TAG3",
    0x10003080: "GIF_CNT", 0x10003090: "GIF_P3CNT", 0x100030A0: "GIF_P3TAG",
    0x10003800: "VIF0_STAT", 0x10003810: "VIF0_FBRST", 0x10003820: "VIF0_ERR", 0x10003830: "VIF0_MARK",
    0x10003840: "VIF0_CYCLE", 0x10003850: "VIF0_MODE", 0x10003860: "VIF0_NUM", 0x10003870: "VIF0_MASK",
    0x10003880: "VIF0_CODE", 0x10003890: "VIF0_ITOPS", 0x100038D0: "VIF0_ITOP",
    0x10003C00: "VIF1_STAT", 0x10003C10: "VIF1_FBRST", 0x10003C20: "VIF1_ERR", 0x10003C30: "VIF1_MARK",
    0x10003C40: "VIF1_CYCLE", 0x10003C50: "VIF1_MODE", 0x10003C60: "VIF1_NUM", 0x10003C70: "VIF1_MASK",
    0x10003C80: "VIF1_CODE", 0x10003C90: "VIF1_ITOPS", 0x10003CA0: "VIF1_BASE", 0x10003CB0: "VIF1_OFST",
    0x10003CC0: "VIF1_TOPS", 0x10003CD0: "VIF1_ITOP", 0x10003CE0: "VIF1_TOP",
    0x10004000: "VIF0_FIFO", 0x10005000: "VIF1_FIFO", 0x10006000: "GIF_FIFO",
    0x10007000: "IPU_OUT_FIFO", 0x10007010: "IPU_IN_FIFO",
    0x1000E000: "D_CTRL", 0x1000E010: "D_STAT", 0x1000E020: "D_PCR", 0x1000E030: "D_SQWC",
    0x1000E040: "D_RBSR", 0x1000E050: "D_RBOR", 0x1000E060: "D_STADR",
    0x1000F000: "I_STAT", 0x1000F010: "I_MASK",
    0x1000F100: "SIO_LCR", 0x1000F110: "SIO_LSR", 0x1000F120: "SIO_IER", 0x1000F130: "SIO_ISR",
    0x1000F140: "SIO_FCR", 0x1000F180: "SIO_TXFIFO", 0x1000F1C0: "SIO_RXFIFO",
    0x1000F200: "SB_MSCOM", 0x1000F210: "SB_SMCOM", 0x1000F220: "SB_MSFLG", 0x1000F230: "SB_SMFLG",
    0x1000F240: "SB_CTRL",
    0x1000F520: "D_ENABLER", 0x1000F590: "D_ENABLEW",
    0x12000000: "GS_PMODE", 0x12000010: "GS_SMODE1", 0x12000020: "GS_SMODE2", 0x12000030: "GS_SRFSH",
    0x12000040: "GS_SYNCH1", 0x12000050: "GS_SYNCH2", 0x12000060: "GS_SYNCV",
    0x12000070: "GS_DISPFB1", 0x12000080: "GS_DISPLAY1", 0x12000090: "GS_DISPFB2", 0x120000A0: "GS_DISPLAY2",
    0x120000B0: "GS_EXTBUF", 0x120000C0: "GS_EXTDATA", 0x120000D0: "GS_EXTWRITE", 0x120000E0: "GS_BGCOLOR",
    0x12001000: "GS_CSR", 0x12001010: "GS_IMR", 0x12001040: "GS_BUSDIR", 0x12001080: "GS_SIGLBLID",
}
for _base, _ch in DMA_CHANNELS.items():
    for _off, _r in DMA_REGS.items():
        REGS[_base + _off] = f"{_ch}_{_r}"


def region(addr):
    """(region, register or offset name) for a hardware address, or None"""
    if 0x10000000 <= addr < 0x10010000:
        name = REGS.get(addr)
        if name is None:
            base = addr & ~0xF
            name = REGS.get(base, "")
            if name:
                name += f"+{addr - base:X}"
        return "ee_reg", name
    if 0x11000000 <= addr < 0x11010000:
        for lo, size, n in ((0x11000000, 0x1000, "VU0_MICRO"), (0x11004000, 0x1000, "VU0_DATA"),
                            (0x11008000, 0x4000, "VU1_MICRO"), (0x1100C000, 0x4000, "VU1_DATA")):
            if lo <= addr < lo + size:
                return "vu_mem", n + (f"+{addr - lo:X}" if addr != lo else "")
        return "vu_mem", ""
    if 0x12000000 <= addr < 0x12002000:
        name = REGS.get(addr) or REGS.get(addr & ~0xF, "")
        return "gs_priv", name
    if 0x70000000 <= addr < 0x70004000:
        return "spr", f"SPR+{addr - 0x70000000:X}"
    if 0x20000000 <= addr < 0x22000000:
        return "uncached", f"RAM {addr - 0x20000000:#x}"
    if 0x30000000 <= addr < 0x32000000:
        return "uncached_acc", f"RAM {addr - 0x30000000:#x}"
    return None


def mechanism(reg_region, reg, access):
    """What the port needs for one access (see the module docstring)"""
    if reg_region in ("spr", "vu_mem"):
        return "buffer"
    if reg_region in ("uncached", "uncached_acc"):
        return "heap"
    r = reg.split("+")[0]
    if reg_region == "gs_priv":
        # CSR: vsync/field/FINISH bits the game polls, writes that reset
        # or acknowledge. The display registers are only state the
        # renderer reads at vsync.
        return "active" if r == "GS_CSR" else "buffer"
    m = re.match(r"D(\d|n)_(\w+)$", r)
    if m:
        if m.group(2) == "CHCR":
            return "active"  # a write with STR starts a transfer, reads poll it
        return "buffer"      # latched; the platform reads them on a start
    if r in ("D_STAT", "I_STAT", "IPU_CMD", "IPU_CTRL", "IPU_BP", "IPU_TOP", "GIF_STAT",
             "VIF0_STAT", "VIF1_STAT", "SIO_TXFIFO", "SIO_ISR", "SIO_LSR") or r.endswith("_FIFO"):
        return "active"
    if r in ("GIF_CTRL", "VIF0_FBRST", "VIF1_FBRST") and "W" in access:
        return "active"  # resets
    if r.startswith("T") and "_" in r and r.endswith("COUNT") and "R" in access:
        return "active"  # free-running counters: a buffer reads a constant
    return "buffer"


# ---- a small C tokenizer -------------------------------------------------

TOKEN = re.compile(r"""
    (?P<ws>[ \t\r\f\v]+|\\\n)
  | (?P<nl>\n)
  | (?P<comment>/\*.*?\*/|//[^\n]*)
  | (?P<str>"(?:\\.|[^"\\\n])*"|'(?:\\.|[^'\\\n])*')
  | (?P<num>(?:0[xX][0-9A-Fa-f]+|\d+\.?\d*(?:[eE][-+]?\d+)?|\.\d+(?:[eE][-+]?\d+)?)[uUlLfF]*)
  | (?P<id>[A-Za-z_]\w*)
  | (?P<op>->|\+\+|--|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=|\#\#|[-+*/%&|^~!<>=?:;,.(){}\[\]\#])
  | (?P<other>.)
""", re.S | re.X)


class Tok:
    __slots__ = ("kind", "text", "line", "pos")

    def __init__(self, kind, text, line, pos=0):
        self.kind, self.text, self.line, self.pos = kind, text, line, pos

    def __repr__(self):
        return f"{self.text}@{self.line}"


def tokenize(src):
    """(code tokens, [(line, directive tokens)]): preprocessor lines apart"""
    code, directives = [], []
    line = 1
    cur = None          # tokens of the directive being read, or None
    at_bol = True
    for m in TOKEN.finditer(src):
        kind = m.lastgroup
        text = m.group()
        if kind == "nl":
            if cur is not None:
                directives.append(cur)
                cur = None
            line += 1
            at_bol = True
            continue
        if kind == "ws":
            line += text.count("\n")
            continue
        if kind == "comment":
            line += text.count("\n")
            continue
        if kind == "op" and text == "#" and at_bol and cur is None:
            cur = (line, [])
            at_bol = False
            continue
        at_bol = False
        t = Tok(kind, text, line, m.start())
        if cur is not None:
            cur[1].append(t)
        else:
            code.append(t)
    if cur is not None:
        directives.append(cur)
    return code, directives


def num_value(text):
    t = text.rstrip("uUlL")
    if t[:2] in ("0x", "0X"):
        return int(t, 16)
    return None


def match_back(toks, i):
    """index of the ( or [ matching the ) or ] at i"""
    close = toks[i].text
    open_ = {")": "(", "]": "[", "}": "{"}[close]
    depth = 0
    while i >= 0:
        t = toks[i].text
        if t == close:
            depth += 1
        elif t == open_:
            depth -= 1
            if depth == 0:
                return i
        i -= 1
    return -1


def match_fwd(toks, i):
    open_ = toks[i].text
    close = {"(": ")", "[": "]", "{": "}"}[open_]
    depth = 0
    while i < len(toks):
        t = toks[i].text
        if t == open_:
            depth += 1
        elif t == close:
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return len(toks) - 1


# & 0x0FFFFFFF on things that aren't addresses (debug menu flag words)
MASK_NOT_ADDRESS = {"src/main/4014F0.c"}
# Masks the game applies to addresses: physical (0x0FFFFFFF), the GS
# texture-transfer address split (0x1FFFFFFF, m:4572C0), and the movie
# code's alignment of heap pointers (0x07FFFFC0, 0x07FFFF80)
ADDRESS_MASKS = {0x0FFFFFFF, 0x1FFFFFFF, 0x07FFFFC0, 0x07FFFF80}
# | 0x30000000 that builds a CHCR value (the tag ID bits), not an address:
# viBufAddDMA reads D4_CHCR into "madr" and restarts the channel with it
ALIAS_NOT_ADDRESS = {("src/main/466848.c", "viBufAddDMA")}

INT_TYPES = {"u8", "u16", "u32", "u64", "u128", "s8", "s16", "s32", "s64", "s128", "int", "long",
             "short", "char", "unsigned", "signed", "f32", "f64", "float", "double", "uintptr_t",
             "intptr_t", "size_t", "u_int", "u_long", "u_short", "u_char", "volatile", "const"}
TYPE_WORDS = INT_TYPES | {"void", "struct", "union", "enum"}


def is_cast(toks, j, k):
    """toks[j..k] is ( ... ): a cast to a pointer type?"""
    inner = toks[j + 1:k]
    if not inner or any(t.kind not in ("id",) and t.text not in ("*", "(", ")") for t in inner):
        return False
    if any(t.text == "*" for t in inner):
        return True
    # (EntFn)0x708000: a typedef that isn't an integer type
    ids = [t.text for t in inner if t.kind == "id"]
    return bool(ids) and not all(w in INT_TYPES for w in ids) and len(ids) == 1


ASSIGN = {"=", "|=", "&=", "+=", "-=", "^=", "*=", "/=", "%=", "<<=", ">>="}
UNARY_PRE = {None, "(", "[", ",", "=", "return", "{", "}", ";", "else", "do", "case", "!", "~", "&&", "||", "?", ":",
             "+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "!=", "&", "|", "^", "<<", ">>"} | ASSIGN


def postfix_end(toks, e):
    """extend an operand ending at e over [..], ->x, .x"""
    while e + 1 < len(toks):
        t = toks[e + 1].text
        if t == "[":
            e = match_fwd(toks, e + 1)
        elif t in ("->", ".") and e + 2 < len(toks):
            e += 2
        else:
            break
    return e


def access_of(toks, s, e, deref):
    """(access, extra) for the operand toks[s..e]"""
    prev = toks[s - 1].text if s > 0 else None
    nxt = toks[e + 1].text if e + 1 < len(toks) else None
    if prev == "&" and (s < 2 or toks[s - 2].text in UNARY_PRE or toks[s - 2].kind == "id"
                        and toks[s - 2].text == "return"):
        return "ptr", "address-of"
    if not deref:
        return "ptr", pointer_use(toks, s, e)
    if prev in ("++", "--") or nxt in ("++", "--"):
        return "RW", ""
    if nxt == "=":
        return "W", ""
    if nxt in ASSIGN:
        return "RW", ""
    return "R", ""


def pointer_use(toks, s, e):
    """where an address that isn't dereferenced goes"""
    prev = toks[s - 1].text if s > 0 else None
    if prev == "=" and s >= 2:
        # var = (T *)ADDR; walk back over the lvalue
        k = s - 2
        if toks[k].text in ("]", ")"):
            k = match_back(toks, k)
        return f"-> {toks[k].text}" if toks[k].kind == "id" else "-> var"
    # an argument: find the call's open paren
    depth = 0
    k = s - 1
    while k >= 0:
        t = toks[k].text
        if t in (")", "]"):
            depth += 1
        elif t in ("(", "["):
            if depth == 0:
                if t == "(" and k > 0 and toks[k - 1].kind == "id":
                    return f"arg of {toks[k - 1].text}"
                break
            depth -= 1
        elif t in (";", "{", "}") and depth == 0:
            break
        k -= 1
    return ""


def expr_left(toks, i):
    """start index of the left operand of the binary operator at i"""
    k = i - 1
    while k >= 0:
        t = toks[k].text
        if t in (")", "]"):
            k = match_back(toks, k) - 1
            continue
        if t in ("(", "[", ",", ";", "{", "}", "=", "return", "?", ":") or t in ASSIGN:
            break
        if t in ("|", "||", "&&"):
            break
        k -= 1
    return k + 1


def expr_right(toks, i):
    k = i + 1
    while k < len(toks):
        t = toks[k].text
        if t in ("(", "["):
            k = match_fwd(toks, k) + 1
            continue
        if t in (")", "]", ",", ";", "}", "?", ":", "|", "||", "&&") or t in ASSIGN:
            break
        k += 1
    return k - 1


def has_top_shift(toks, s, e):
    depth = 0
    for t in toks[s:e + 1]:
        if t.text in ("(", "["):
            depth += 1
        elif t.text in (")", "]"):
            depth -= 1
        elif depth <= 1 and t.text in ("<<", ">>"):
            return True
    return False


def functions_of(toks):
    """[(start, end, name)] of function bodies"""
    out = []
    i = 0
    n = len(toks)
    while i < n:
        t = toks[i]
        if t.text == "{":
            j = i - 1
            name = None
            if j >= 0 and toks[j].text == ")":
                k = match_back(toks, j)
                # void (*f(void))(int) is rare here; take the id before (
                if k > 0 and toks[k - 1].kind == "id" and toks[k - 1].text not in ("if", "while", "for", "switch"):
                    if k < 2 or toks[k - 2].text != "=":
                        name = toks[k - 1].text
            end = match_fwd(toks, i)
            if name:
                out.append((i, end, name))
            i = end + 1
            continue
        i += 1
    return out


# ---- macros ---------------------------------------------------------------

class Macro:
    def __init__(self, name, params, body, line, path):
        self.name, self.params, self.body, self.line, self.path = name, params, body, line, path
        self.addrs = []     # hardware literals in the body: (addr, deref)
        self.alias = False  # | 0x20000000 / 0x30000000 arithmetic on a parameter
        self.deref_param = False  # *(T *)(param): REG(a)
        self.deref = False


def parse_macro(line, dtoks, path):
    if len(dtoks) < 2 or dtoks[0].text != "define":
        return None
    name = dtoks[1].text
    rest = dtoks[2:]
    params = None
    # function-like only if ( follows the name without a space
    if rest and rest[0].text == "(" and rest[0].pos == dtoks[1].pos + len(name):
        k = match_fwd(rest, 0)
        params = [t.text for t in rest[1:k] if t.kind == "id"]
        body = rest[k + 1:]
    else:
        body = rest
    mac = Macro(name, params, body, line, path)
    body_deref = bool(body) and (body[0].text == "*" or len(body) > 1 and body[0].text == "(" and body[1].text == "*")
    mac.deref = body_deref
    for i, t in enumerate(body):
        if t.kind == "num":
            v = num_value(t.text)
            if v is None:
                continue
            if v in (0x20000000, 0x30000000) and (i > 0 and body[i - 1].text in ("|", "+")):
                mac.alias = True
                continue
            if region(v) and region(v)[0] not in ("uncached", "uncached_acc"):
                # cast directly before it, or (base + (n) * k)
                cast = i > 0 and body[i - 1].text == ")" and is_cast(body, match_back(body, i - 1), i - 1)
                inner = i > 1 and body[i - 1].text == "(" and body[i - 2].text == ")"
                if cast or inner:
                    mac.addrs.append(v)
        if params and t.kind == "id" and t.text in params and body_deref:
            mac.deref_param = True
    return mac


# ---- the scan --------------------------------------------------------------

class Site:
    __slots__ = ("path", "line", "func", "region", "addr", "reg", "access", "kind", "mech", "sdk", "note")

    def __init__(self, **kw):
        for k in self.__slots__:
            setattr(self, k, kw.get(k, ""))

    def row(self):
        a = f"0x{self.addr:08X}" if isinstance(self.addr, int) else self.addr
        return "\t".join([f"{self.path}:{self.line}", self.func, self.region, a, self.reg,
                          self.access, self.kind, self.mech, "sdk" if self.sdk else "", self.note])


def in_loop_condition(toks, i):
    """is toks[i] inside the condition of a while or for?"""
    depth = 0
    k = i - 1
    while k >= 0:
        t = toks[k].text
        if t in (")", "]"):
            depth += 1
        elif t in ("(", "["):
            if depth == 0:
                if t == "(" and k > 0 and toks[k - 1].text in ("while", "for"):
                    return True
            else:
                depth -= 1
        elif t in (";", "{", "}") and depth == 0:
            return False
        k -= 1
    return False


def func_at(funcs, idx):
    for s, e, name in funcs:
        if s <= idx <= e:
            return name
    return "(file scope)"


LINKER_NAMES = None


def linker_names():
    """{name: (address, mechanism, where)} the port's C may use by name"""
    global LINKER_NAMES
    if LINKER_NAMES is not None:
        return LINKER_NAMES
    out = {}
    for path in sorted(glob.glob(os.path.join(ROOT, "build", "c_syms*.ld"))):
        with open(path) as f:
            for m in re.finditer(r"PROVIDE\(([\w.]+) = (0x[0-9A-Fa-f]+)\);", f.read()):
                a = int(m.group(2), 16)
                if not 0x00400000 <= a < 0x02000000:
                    out.setdefault(m.group(1), (a, os.path.relpath(path, ROOT).replace("\\", "/")))
    with open(os.path.join(ROOT, "config", "memory_map.ld")) as f:
        for m in re.finditer(r"^(\w+)\s*=\s*(0x[0-9A-Fa-f]+|\w+)\s*;", f.read(), re.M):
            v = m.group(2)
            if v.startswith("0x"):
                out.setdefault(m.group(1), (int(v, 16), "config/memory_map.ld"))
            elif v in out:
                out.setdefault(m.group(1), (out[v][0], "config/memory_map.ld"))
    LINKER_NAMES = out
    return out


def fixed_mech(name, addr):
    if region(addr):
        return region(addr)[0], region(addr)[1]
    if 0x005B0000 <= addr < 0x00750000 or name.endswith("_VRAM") or name.startswith("func_"):
        return "fixed_ram", "load area"
    return "fixed_ram", ""


def scan_file(rel, sites, values, header_macros):
    with open(os.path.join(ROOT, rel), encoding="latin-1") as f:
        src = f.read()
    sdk = "Sony PS2 SDK library code" in src[:600] or "Third-party library code" in src[:600]
    toks, directives = tokenize(src)
    funcs = functions_of(toks)
    macros = dict(header_macros)
    for line, dt in directives:
        m = parse_macro(line, dt, rel)
        if m and (m.addrs or m.alias or m.deref_param):
            macros[m.name] = m
            for a in m.addrs:
                r, reg = region(a)
                sites.append(Site(path=rel, line=line, func=f"#define {m.name}", region=r, addr=a, reg=reg,
                                  access="def", kind="macro", mech="", sdk=sdk))
    names = linker_names()

    def add(i, addr, access, kind, note="", reg_region=None, reg=None):
        if reg_region is None:
            reg_region, reg = region(addr)
        mech = mechanism(reg_region, reg, access)
        if kind == "alias":
            mech = "alias"
        elif kind in ("range", "mask"):
            mech = "phys"
        if access == "R" and in_loop_condition(toks, i):
            note = (note + " poll").strip()
        sites.append(Site(path=rel, line=toks[i].line, func=func_at(funcs, i), region=reg_region, addr=addr,
                          reg=reg, access=access, kind=kind, mech=mech, sdk=sdk, note=note))

    n = len(toks)
    for i, t in enumerate(toks):
        if t.kind == "id":
            if t.text in names:
                addr, where = names[t.text]
                # skip the declaration itself (extern ... NAME;)
                prev = toks[i - 1].text if i else ""
                if prev in ("extern",) or (i + 1 < n and toks[i + 1].text in (";", "[") and
                                           toks[i - 1].kind == "id" and toks[i - 1].text in TYPE_WORDS | {"EntFn"}):
                    continue
                if any(s <= i <= e for s, e, _ in funcs) or toks[i - 1].text in ("{", ","):
                    r, reg = fixed_mech(t.text, addr)
                    mech = "load" if reg == "load area" else ("buffer" if r != "fixed_ram" else "heap")
                    if t.text == "btlDbgStage":
                        mech, reg = "buffer", "crash marker word"
                    e = postfix_end(toks, i)
                    acc, extra = access_of(toks, i, e, True) if t.text == "btlDbgStage" else ("ptr", pointer_use(toks, i, e))
                    sites.append(Site(path=rel, line=t.line, func=func_at(funcs, i), region=r, addr=addr, reg=reg,
                                      access=acc, kind="linker", mech=mech, sdk=sdk,
                                      note=f"{t.text} ({where}) {extra}".strip()))
                continue
            m = macros.get(t.text)
            if m is None:
                continue
            if m.params is not None:
                if i + 1 >= n or toks[i + 1].text != "(":
                    continue
                e = match_fwd(toks, i + 1)
            else:
                e = i
            if m.addrs:
                deref = m.deref
                e2 = postfix_end(toks, e)
                deref = deref or e2 > e
                s0 = i
                if not deref and i > 0 and toks[i - 1].text == "*" and (i < 2 or toks[i - 2].text in UNARY_PRE):
                    s0, deref = i - 1, True  # *PTR_MACRO
                acc, extra = access_of(toks, s0, e2, deref)
                for a in m.addrs:
                    add(i, a, acc, "macro", f"{m.name} {extra}".strip())
            elif m.alias:
                e2 = postfix_end(toks, e)
                acc, extra = access_of(toks, i, e2, m.deref or e2 > e)
                add(i, 0x20000000, acc, "alias", f"{m.name} {extra}".strip(), "uncached", "")
            continue
        if t.kind != "num":
            continue
        v = num_value(t.text)
        if v is None:
            continue
        prev = toks[i - 1].text if i else None
        nxt = toks[i + 1].text if i + 1 < n else None

        # & 0x0FFFFFFF and the like: an address masked for DMA (not flag words)
        if v in ADDRESS_MASKS and prev == "&":
            left = toks[expr_left(toks, i - 1):i - 1]
            if any("flag" in tk.text.lower() for tk in left) or rel in MASK_NOT_ADDRESS:
                values.append((rel, t.line, func_at(funcs, i), v, "flag mask, not an address"))
            else:
                add(i, v, "ptr", "mask", "", "phys", "")
            continue
        # SPR address + 0x10000000 (or + 0x90000000 | 0x80000000): the MADR SPR bit
        if v in (0x10000000, 0x90000000) and prev in ("+", "+="):
            add(i, v, "ptr", "range", "scratchpad address to DMA SPR flag", "phys", "")
            continue
        # | 0x20000000, | 0x30000000 (and +): uncached aliases
        if v in (0x20000000, 0x30000000) and (prev in ("|", "+") or nxt in ("|", "+")):
            op = i - 1 if prev in ("|", "+") else i + 1
            if op == i - 1:
                s = expr_left(toks, op)
                other = (s, op - 1)
            else:
                other = (op + 1, expr_right(toks, op))
            if (has_top_shift(toks, *other) or any(tk.text == "SET_FLOAT_WORD" for tk in toks[max(0, i - 8):i])
                    or (rel, func_at(funcs, i)) in ALIAS_NOT_ADDRESS):
                values.append((rel, t.line, func_at(funcs, i), v, "command/bit pattern, not an address"))
                continue
            # the whole a | 0x20000000 and what is done with it
            s = min(other[0], i)
            e = max(other[1], i)
            while s > 0 and toks[s - 1].text == "(" and e + 1 < n and toks[e + 1].text == ")":
                s, e = s - 1, e + 1
            deref = False
            if s > 0 and toks[s - 1].text == ")":
                cs = match_back(toks, s - 1)
                if is_cast(toks, cs, s - 1):
                    s = cs
                    while s > 0 and toks[s - 1].text == "(" and e + 1 < n and toks[e + 1].text == ")":
                        s, e = s - 1, e + 1
                    if s > 0 and toks[s - 1].text == "*" and (s < 2 or toks[s - 2].text in UNARY_PRE):
                        s -= 1
                        deref = True
            e2 = postfix_end(toks, e)
            acc, extra = access_of(toks, s, e2, deref or e2 > e)
            add(i, v, acc, "alias", extra, "uncached" if v == 0x20000000 else "uncached_acc", "")
            continue
        r = region(v)
        if r is None:
            # fixed RAM literals cast to pointers: (EntFn)0x708000, MemInit(.., 0x760000, 0x1FF0000)
            if 0x00700000 <= v < 0x02000000 and prev == ")" and is_cast(toks, match_back(toks, i - 1), i - 1):
                s = match_back(toks, i - 1)
                add(i, v, "ptr", "cast", pointer_use(toks, s, i), "fixed_ram",
                    "load area" if v < 0x750000 else "")
                sites[-1].mech = "load" if v < 0x750000 else "heap"
            elif v in (0x760000, 0x1FF0000) and any(tk.text == "MemInit" for tk in toks[max(0, i - 8):i]):
                add(i, v, "ptr", "arg", "arg of MemInit (the game's heap)", "fixed_ram", "heap")
                sites[-1].mech = "heap"
            continue
        reg_region, reg = r
        # a cast: (T *)ADDR, possibly ((T *)ADDR), *(T *)ADDR, ((T *)ADDR)[i]
        if prev == ")":
            cs = match_back(toks, i - 1)
            if is_cast(toks, cs, i - 1):
                s, e = cs, i
                while s > 0 and toks[s - 1].text == "(" and e + 1 < n and toks[e + 1].text == ")":
                    s, e = s - 1, e + 1
                deref = False
                if s > 0 and toks[s - 1].text == "*" and (s < 2 or toks[s - 2].text in UNARY_PRE
                                                          or toks[s - 2].kind == "id" and toks[s - 2].text == "return"):
                    s -= 1
                    deref = True
                e2 = postfix_end(toks, e)
                acc, extra = access_of(toks, s, e2, deref or e2 > e)
                add(i, v, acc, "cast", extra, reg_region, reg)
                continue
        # REG(0x...), SPR-style function-like macros that cast their argument
        if prev == "(" and i > 1 and toks[i - 2].text in macros and macros[toks[i - 2].text].deref_param:
            e = match_fwd(toks, i - 1)
            e2 = postfix_end(toks, e)
            acc, extra = access_of(toks, i - 2, e2, True)
            add(i, v, acc, "macro", f"{toks[i - 2].text}()", reg_region, reg)
            continue
        if reg_region == "spr":
            if prev in ("<", ">", "<=", ">=", "-", "==", "!=") or nxt in ("<", ">", "<=", ">=", "==", "!="):
                add(i, v, "ptr", "range", "scratchpad range check/offset", reg_region, reg)
                continue
            use = pointer_use(toks, i, i)
            if use.startswith("arg of"):
                add(i, v, "ptr", "arg", use, reg_region, reg)
                continue
        if reg_region == "spr" and prev == "&" and nxt == ")":
            add(i, v, "ptr", "range", "scratchpad address test", reg_region, reg)
            continue
        in_func = any(fs <= i <= fe for fs, fe, _ in funcs)
        if reg_region == "spr" and v & 0xF == 0 and (in_func or prev == "=" and nxt == ";"):
            # a scratchpad pointer as a plain value (stored, compared,
            # added to): unless it is the value written to a register
            near = [num_value(tk.text) for tk in toks[max(0, i - 8):i] if tk.kind == "num"]
            if not any(x is not None and region(x) and region(x)[0] == "ee_reg" for x in near):
                add(i, v, "ptr", "value", pointer_use(toks, i, i), reg_region, reg)
                continue
        if reg_region == "ee_reg" and not in_func and v in REGS and v != 0x10000000 and v & 0xF == 0:
            # register addresses in a data table (libmpeg's, libdma's channel list)
            add(i, v, "ptr", "table", "register address in a data table", reg_region, reg)
            continue
        if v == 0x6FFFFFFF:
            add(i, 0x70000000, "ptr", "range", "scratchpad range check", "spr", "SPR+0")
            continue
        values.append((rel, t.line, func_at(funcs, i), v, " ".join(x.text for x in toks[max(0, i - 4):i + 3])))

    # Registers reached through a local pointer (volatile u32 *c = (volatile
    # u32 *)0x1000B420; *c = ...): follow the name inside the function
    for site in [x for x in sites if x.path == rel and x.note.startswith("-> ")
                 and x.region in ("ee_reg", "gs_priv", "vu_mem")]:
        var = site.note[3:]
        for fs, fe, fname in funcs:
            if fname != site.func:
                continue
            for i in range(fs, fe):
                if toks[i].text != var or toks[i].line <= site.line:
                    continue
                if i + 1 < n and toks[i + 1].text == "=" and toks[i - 1].text not in ("*",):
                    break  # reassigned
                s0, deref = i, False
                if toks[i - 1].text == "*" and toks[i - 2].text in UNARY_PRE:
                    s0, deref = i - 1, True
                e = postfix_end(toks, i)
                deref = deref or e > i
                if not deref:
                    continue
                acc, _ = access_of(toks, s0, e, True)
                add(i, site.addr, acc, "pointer", f"through {var}", site.region, site.reg)

    # DMA channels reached through sceDmaChan pointers (sceDmaGetChan, the
    # gsDmaChan* globals, _sceDmaChanTable): d->chcr, d->madr, ...
    if re.search(r"sceDmaChan|gsDmaChan|_sceDmaChanTable|DmaCh\b", src):
        fields = {"chcr": "CHCR", "madr": "MADR", "qwc": "QWC", "tadr": "TADR", "as0": "ASR0", "as1": "ASR1",
                  "asr0": "ASR0", "asr1": "ASR1", "sadr": "SADR"}
        chans = {"gsDmaChanVif0": "D0", "gsDmaChanVif1": "D1", "gsDmaChanGif": "D2",
                 "gsDmaChanFromSpr": "D8", "gsDmaChanToSpr": "D9"}
        for i in range(1, n - 1):
            if toks[i].text != "->" or toks[i + 1].text not in fields:
                continue
            k = i - 1
            if toks[k].text == "]":
                k = match_back(toks, k) - 1
            base = toks[k].text
            ch = chans.get(base, "Dn")
            reg = f"{ch}_{fields[toks[i + 1].text]}"
            e = postfix_end(toks, i - 1)
            s0 = k
            # *(volatile u32 *)&d->chcr
            if s0 >= 1 and toks[s0 - 1].text == "&" and toks[s0 - 2].text == ")":
                cs = match_back(toks, s0 - 2)
                if is_cast(toks, cs, s0 - 2) and toks[cs - 1].text == "*":
                    s0 = cs - 1
            acc, _ = access_of(toks, s0, e, True)
            addr = DMA_CHANNEL_BY_NAME.get(reg, "Dn")
            add(i, addr, acc, "channel", f"{base}->{toks[i + 1].text}", "ee_reg", reg)


DMA_CHANNEL_BY_NAME = {v: k for k, v in REGS.items()}


def header_macros():
    """hardware macros defined in include/ (none at the time of writing)"""
    out = {}
    for p in glob.glob(os.path.join(ROOT, "include", "**", "*.h"), recursive=True):
        with open(p, encoding="latin-1") as f:
            _, directives = tokenize(f.read())
        for line, dt in directives:
            m = parse_macro(line, dt, os.path.relpath(p, ROOT).replace("\\", "/"))
            if m and (m.addrs or m.alias or m.deref_param):
                out[m.name] = m
    return out


def sources():
    files = []
    for pattern in ("src/*/*.c", "src/*/data/*.c", "port/game/*/*.c", "include/**/*.h"):
        files += glob.glob(os.path.join(ROOT, pattern), recursive=True)
    return sorted(os.path.relpath(p, ROOT).replace("\\", "/") for p in files
                  if "pending_inline_asm" not in p)


def scan():
    sites, values = [], []
    hm = header_macros()
    for rel in sources():
        scan_file(rel, sites, values, hm)
    return sites, values


# ---- functions still in assembly ------------------------------------------

ASM_ADDR = re.compile(r"\((0x[0-9A-Fa-f]{8}) (?:>> 16|& 0xFFFF)\)")
ASM_INSN = re.compile(r"\s*/\* \w+ [0-9A-F]{8} [0-9A-F]{8} \*/\s+(\S+)\s+(.*)")
LOADS = {"lb", "lbu", "lh", "lhu", "lw", "lwu", "ld", "lq", "lwc1", "ldc1", "lqc2", "lwl", "lwr", "ldl", "ldr"}
STORES = {"sb", "sh", "sw", "sd", "sq", "swc1", "sdc1", "sqc2", "swl", "swr", "sdl", "sdr"}


def port_defined():
    """names port/game/ defines (C for functions still in assembly)"""
    out = set()
    for p in glob.glob(os.path.join(ROOT, "port", "game", "*", "*.c")):
        with open(p, encoding="latin-1") as f:
            out.update(re.findall(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{", f.read(), re.M))
    return out


def scan_asm():
    """[(asm file, function, address, region, register, access, has port C)]:
    loads and stores whose base register holds a hardware address (formed
    by lui/ori/addiu, which splat writes with the full value)"""
    out = []
    names = linker_names()
    have = port_defined()
    mem = re.compile(r"\$(\w+),\s*(.+)\(\$(\w+)\)$")
    for path in sorted(glob.glob(os.path.join(ROOT, "asm", "*", "*", "*.s"))):
        if os.sep + "data" + os.sep in path:
            continue
        rel = os.path.relpath(path, ROOT).replace("\\", "/")
        func = None
        regs = {}
        with open(path, encoding="latin-1") as f:
            for line in f:
                m = re.match(r"glabel (\w+)", line)
                if m:
                    func, regs = m.group(1), {}
                    continue
                m = ASM_INSN.match(line)
                if not m or func is None:
                    continue
                ins, ops = m.group(1), m.group(2).split("/*")[0].strip()
                if ins in LOADS or ins in STORES:
                    mm = mem.match(ops)
                    if not mm:
                        continue
                    rt, off, base = mm.groups()
                    a = ASM_ADDR.search(off)
                    if a:
                        addr = int(a.group(1), 16)
                    elif base in regs:
                        try:
                            addr = regs[base] + int(off, 0)
                        except ValueError:
                            lo = re.search(r"%lo\((\w+)\)", off)
                            addr = names[lo.group(1)][0] if lo and lo.group(1) in names else None
                    else:
                        addr = None
                    if ins in LOADS:
                        regs.pop(rt, None)
                    if addr is None or region(addr) is None:
                        continue
                    r, reg = region(addr)
                    out.append((rel, func, addr, r, reg, "R" if ins in LOADS else "W", func in have))
                    continue
                dest = re.match(r"\$(\w+)", ops)
                if not dest:
                    continue
                dest = dest.group(1)
                a = ASM_ADDR.search(ops)
                hl = re.search(r"%(?:hi|lo)\((\w+)\)", ops)
                if ins in ("lui", "ori", "addiu", "daddiu") and (a or hl and hl.group(1) in names):
                    v = int(a.group(1), 16) if a else names[hl.group(1)][0]
                    regs[dest] = v
                    if region(v) is None:
                        regs.pop(dest, None)
                elif ins in ("daddu", "addu", "or") and re.match(r"\$\w+,\s*\$(\w+),\s*\$zero$", ops):
                    src = re.match(r"\$\w+,\s*\$(\w+)", ops).group(1)
                    if src in regs:
                        regs[dest] = regs[src]
                    else:
                        regs.pop(dest, None)
                else:
                    regs.pop(dest, None)
    return out


# ---- output ------------------------------------------------------------

def per_function(sites):
    funcs = defaultdict(list)
    for s in sites:
        if s.access == "def":
            continue
        funcs[(s.path, s.func)].append(s)
    return funcs


def replace_reasons(ss):
    """why a function can't run on mapped memory alone, in the design of
    port/docs/hardware.md: DMA runs synchronously inside platform
    calls and writes the channel registers back (STR clear, QWC 0, MADR and
    TADR advanced), so polls of CHCR/QWC/MADR/STAT see a finished transfer
    and need nothing; what starts work, talks to the IPU or a FIFO, or
    waits for time to pass does."""
    out = []
    for s in ss:
        r = (s.reg or "").split("+")[0]
        if re.match(r"D[\dn]_CHCR$", r) and s.access in ("W", "RW"):
            why = "starts/stops DMA (CHCR)"
        elif r.endswith("_FIFO"):
            why = "FIFO"
        elif r.startswith("IPU_"):
            why = "IPU"
        elif r in ("GIF_CTRL", "VIF0_FBRST", "VIF1_FBRST") and s.access in ("W", "RW"):
            why = "GIF/VIF reset"
        elif r == "GS_CSR" and s.access in ("W", "RW"):
            why = "GS CSR write"
        elif r in ("GS_CSR", "I_STAT") and "poll" in s.note:
            why = "waits on GS CSR/INTC"
        elif r == "I_STAT" and s.access in ("W", "RW"):
            why = "INTC acknowledge"
        elif r == "SIO_TXFIFO":
            why = "SIO output"
        elif s.region == "uncached" and s.mech == "heap":
            why = "IOP shared block"
        elif s.mech == "load" and s.kind in ("cast",) or s.mech == "load" and "func_" in s.note:
            why = "calls a disc program at its load address"
        elif s.mech == "heap":
            why = "fixed heap range"
        else:
            continue
        if why not in out:
            out.append(why)
    return out


def replacements(sites):
    out = []
    for key, ss in sorted(per_function(sites).items()):
        reasons = replace_reasons(ss)
        if reasons and key[1] != "(file scope)":
            out.append((key, reasons, ss[0].sdk))
    return out


def summary(sites, values):
    use = [s for s in sites if s.access != "def"]
    print(f"{len(use)} sites in {len({(s.path, s.func) for s in use})} functions, "
          f"{len({s.path for s in use})} files ({sum(1 for s in sites if s.access == 'def')} macro definitions); "
          f"{len(values)} literals in the same ranges taken as values (--values)\n")
    print("By region:")
    for r, c in Counter(s.region for s in use).most_common():
        addrs = {s.addr for s in use if s.region == r}
        print(f"  {r:14} {c:5} sites  {len(addrs):4} distinct addresses")
    print("\nBy mechanism (what the port needs):")
    for mech, c in Counter(s.mech for s in use).most_common():
        fs = {(s.path, s.func) for s in use if s.mech == mech}
        sdk = sum(1 for s in use if s.mech == mech and s.sdk)
        print(f"  {mech:8} {c:5} sites  {len(fs):4} functions  ({sdk} sites in SDK code)")
    print("\nBy access:")
    for a, c in Counter(s.access for s in use).most_common():
        print(f"  {a:4} {c:5}")
    hw = {s.addr for s in use if s.region in ("ee_reg", "gs_priv", "vu_mem")}
    print(f"\nDistinct register/VU addresses: {len(hw)}; distinct scratchpad addresses: "
          f"{len({s.addr for s in use if s.region == 'spr'})}")
    print("\nRegisters by use (sites, R/W):")
    regs = defaultdict(Counter)
    for s in use:
        if s.region in ("ee_reg", "gs_priv"):
            regs[s.reg][s.access] += 1
    for reg, c in sorted(regs.items(), key=lambda kv: -sum(kv[1].values())):
        print(f"  {reg:14} {sum(c.values()):4}  " + " ".join(f"{k}:{v}" for k, v in sorted(c.items())))
    print("\nNames the linker fixes (build/c_syms*.ld, config/memory_map.ld):")
    asm_refs = defaultdict(set)
    for path in glob.glob(os.path.join(ROOT, "asm", "*", "*", "*.s")):
        func = None
        with open(path, encoding="latin-1") as f:
            for line in f:
                m = re.match(r"glabel (\w+)", line)
                if m:
                    func = m.group(1)
                for n in re.findall(r"%(?:hi|lo)\((\w+)\)", line):
                    if n in linker_names():
                        asm_refs[n].add(func)
    have = port_defined()
    for name, (addr, where) in sorted(linker_names().items(), key=lambda kv: kv[1]):
        c = [x for x in use if x.kind == "linker" and x.note.startswith(name + " ")]
        a = [f + ("*" if f in have else "") for f in sorted(asm_refs.get(name, ()))]
        print(f"  {name:16} 0x{addr:08X} {where:22} C sites: {len(c):2}  asm: {' '.join(a) or '-'}")
    print("  (* = the function has port C in port/game/)")
    fs = {}
    for rel, func, addr, r, reg, acc, h in scan_asm():
        fs.setdefault((func, h), set()).add(r)
    print(f"\nFunctions still in assembly that access these ranges: {len(fs)} "
          f"({sum(1 for f, h in fs if not h)} without port C yet; --asm)")
    for (func, h), rs in sorted(fs.items()):
        if not h:
            print(f"  {func:30} {' '.join(sorted(rs))}")
    print("\nFunctions with active sites:")
    for (path, func), ss in sorted(per_function(sites).items()):
        act = [s for s in ss if s.mech == "active"]
        if act:
            regs = sorted({f"{s.reg}:{s.access}" for s in act})
            print(f"  {path:24} {func:32} {'sdk' if ss[0].sdk else '   '} {' '.join(regs)}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--sites", action="store_true", help="TSV of every site")
    ap.add_argument("--functions", action="store_true", help="TSV per function")
    ap.add_argument("--values", action="store_true", help="literals in the ranges taken as values")
    ap.add_argument("--asm", action="store_true", help="accesses in functions still in assembly")
    ap.add_argument("--replace", action="store_true",
                    help="functions that need port C replacements (synchronous DMA design), with the reason")
    args = ap.parse_args()
    sites, values = scan()
    if args.sites:
        print("site\tfunction\tregion\taddress\tregister\taccess\tkind\tmechanism\tsdk\tnote")
        for s in sites:
            print(s.row())
    elif args.functions:
        print("file\tfunction\tsdk\tsites\tmechanisms\tregisters")
        for (path, func), ss in sorted(per_function(sites).items()):
            mechs = Counter(s.mech for s in ss)
            regs = sorted({f"{s.reg or s.region}:{s.access}" for s in ss if s.region != "spr"})
            spr = sum(1 for s in ss if s.region == "spr")
            if spr:
                regs.append(f"SPR x{spr}")
            print(f"{path}\t{func}\t{'sdk' if ss[0].sdk else ''}\t{len(ss)}\t"
                  + ",".join(f"{k}:{v}" for k, v in sorted(mechs.items())) + "\t" + " ".join(regs))
    elif args.values:
        print("site\tfunction\tvalue\tcontext")
        for rel, line, func, v, ctx in values:
            print(f"{rel}:{line}\t{func}\t0x{v:08X}\t{ctx}")
    elif args.replace:
        print("file	function	sdk	reasons")
        for (path, func), reasons, sdk in replacements(sites):
            print(f"{path}	{func}	{'sdk' if sdk else ''}	{', '.join(reasons)}")
    elif args.asm:
        rows = scan_asm()
        print("asm\tfunction\taddress\tregion\tregister\taccess\tport C")
        for rel, func, addr, r, reg, acc, have in rows:
            print(f"{rel}\t{func}\t0x{addr:08X}\t{r}\t{reg}\t{acc}\t{'yes' if have else 'no'}")
        fs = {(f, h) for _, f, _, _, _, _, h in rows}
        print(f"# {len(rows)} accesses in {len(fs)} functions, "
              f"{sum(1 for f, h in fs if not h)} without port C yet", file=sys.stderr)
    else:
        summary(sites, values)


if __name__ == "__main__":
    main()
