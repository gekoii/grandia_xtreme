#!/usr/bin/env python3
"""Generate the MIPS interpreter's ABI tables and bridge (port/src/mips/).

    python port/tools/gen_mips_bridge.py           # writes the two files below
    python port/tools/gen_mips_bridge.py --check   # only reports, writes nothing

The disc's programs call 225 functions of the main executable and BATTLE by
address and use their globals by address (config/abi_main.txt,
config/abi_battle.txt). The interpreter (port/src/mips/cpu.c) runs a `jal`
to one of those addresses as a call of the port's C function of that name,
with the EABI arguments marshalled: integer and pointer arguments from
$a0-$a3, $t0-$t3, float arguments from $f12-$f19, each class counted on its
own; the result into $v0 or $f0. The C prototype decides each argument's
class, so this script reads the prototypes:

  1. the function's definition (src/, port/game/): what the callee really
     takes;
  2. else its declaration in include/ (protos/*.h first);
  3. else the declarations in src/ (the most common one);
  4. else OVERRIDES below (functions still in assembly).

An `s32` parameter that the definition casts to a pointer (an `s32` that
holds a callback, BtlHitSetNextVictimHandler) counts as a pointer, so the
interpreter converts it the way it converts pointers (a MIPS code address
becomes a native thunk). OVERRIDES can say so too.

Writes:
  port/src/mips/abi_gen.c     the tables: address, name, signature of every
                              function; EE address, size and name of every
                              global object (sizes from the matching ELFs).
                              No game symbols: the interpreter's tests link it.
  port/src/mips/bridge_gen.c  a handler per function that calls the port's
                              C function, and the host addresses of the
                              functions and globals (grx_mips_install_bridge).

Signatures: "<result>:<params>", one letter each:
  v void   p pointer   f float   d double (soft-float, one 64-bit GPR)
  l 64-bit integer   b s8   B u8   h s16   H u16   i s32   I u32   . varargs

Globals: each data address in the ABI lists is mapped by the object that
holds it in the matching build (build/SLUS_204.17.elf, build/battle.elf),
whole; small gaps between two of them (less than GAP_FILL bytes) are filled
with the objects that lie there, so an index running from one ABI object
into the next lands in the right port object. Only addresses, sizes and our
own names are written, no disc data.
"""
import argparse
import collections
import glob
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import elf32  # noqa: E402

ABI_LISTS = ["config/abi_main.txt", "config/abi_battle.txt"]
ELFS = ["build/SLUS_204.17.elf", "build/battle.elf"]
OUT_ABI = "port/src/mips/abi_gen.c"
OUT_BRIDGE = "port/src/mips/bridge_gen.c"
GAP_FILL = 0x1000

# Signatures the sources don't give (functions still in assembly), and
# corrections. name -> signature
OVERRIDES = {
    # libgcc's soft-float double helpers (fp-bit): doubles are 64-bit
    # values in one GPR; the interpreter has its own (softfp.c)
    "dpadd": "d:dd",
    "dpsub": "d:dd",
    "dpmul": "d:dd",
    "litodp": "d:i",
    "dptoli": "i:d",
    "dptofp": "f:d",
    "fptodp": "d:f",
    # VU0 helpers still in assembly (asm/main/), from their register use:
    # (dst, 4 control points, t) and (dst, src) -> 1/length
    "VuBezierInterp": "v:ppf",
    "VuVecNormalizeRcpLen": "f:pp",
    # s32 parameters and results that hold addresses: kept as they are by
    # the callee and used as pointers later, which the cast scan can't see
    "BtlHitSetNextSrcPos": "v:p",                # a BtlHitSource
    "BtlHitSetNextCallback": "v:pp",             # callback, its argument
    "BtlFileLoadAsync": "p:pp",                  # name, destination (0x748000) -> the buffer
    "BtlFileRelocate": "p:p",                    # a loaded file
    "BtlChrGetCenterY": "f:p",                   # a BtlUnit, handed to BtlChrGetPartPos
    "MemCalloc16A": "p:i",
    "FilePacGetModel": "p:pI",
    # declared void, but the programs read BtlUnitMoveToward's result from
    # $v0 after it: the port's definition returns it (fixups/abi.py)
    "BtlUnitApproachTargetBy": "i:pif",
}
# Functions the interpreter implements itself (port/src/mips/builtins.c):
# name -> C handler. The soft-float helpers, and the VU0 helpers the port
# has no C for yet.
BUILTINS = {n: "grx_mips_bi_" + n for n in ("dpadd", "dpsub", "dpmul", "litodp", "dptoli", "dptofp", "fptodp",
                                            "VuBezierInterp", "VuVecNormalizeRcpLen")}
# Names the host C runtime defines: declared by its headers
LIBC = {"rand": "stdlib.h", "memset": "string.h", "asinf": "math.h", "atan2f": "math.h", "logf": "math.h"}

SCALARS = {
    "f32": "f", "float": "f", "f64": "d", "double": "d",
    "s64": "l", "u64": "l", "s8": "b", "u8": "B", "s16": "h", "u16": "H",
    "s32": "i", "u32": "I", "int": "i", "long": "i", "char": "b", "short": "h",
    "signed": "i", "unsigned": "I", "void": "v",
}
C_TYPES = {"v": "void", "p": "void *", "f": "float", "d": "double", "l": "long long",
           "b": "signed char", "B": "unsigned char", "h": "short", "H": "unsigned short",
           "i": "int", "I": "unsigned int"}
KEYWORDS = {"return", "else", "case", "goto", "do", "sizeof", "if", "while", "for", "switch"}


def strip_comments(t):
    t = re.sub(r"/\*.*?\*/", " ", t, flags=re.S)
    return re.sub(r"//[^\n]*", " ", t)


def read(path):
    with open(path, encoding="latin-1") as f:
        return strip_comments(f.read())


# ------------------------------------------------------------------ ABI lists
def abi_lists():
    funcs, datas = [], []
    for rel in ABI_LISTS:
        for line in open(os.path.join(ROOT, rel)):
            m = re.match(r"0x([0-9A-Fa-f]+) (func|data) (\S+)", line)
            if m:
                (funcs if m.group(2) == "func" else datas).append((int(m.group(1), 16), m.group(3)))
    return sorted(funcs), sorted(datas)


# ------------------------------------------------------------------ types
def typedefs(texts):
    """typedef name -> class letter ('p', 'S' for a struct by value, or a scalar's)"""
    raw = {}
    for t in texts:
        for m in re.finditer(r"\btypedef\b([^;{}]*(?:\{[^{}]*(?:\{[^{}]*\}[^{}]*)*\}[^;{}]*)?);", t):
            body = m.group(1).strip()
            fp = re.search(r"\(\s*\*\s*(\w+)\s*\)\s*\(", body)
            if fp:
                raw[fp.group(1)] = "p"
                continue
            mm = re.match(r"(.*?)\b(\w+)\s*((?:\[[^\]]*\]\s*)*)$", body, re.S)
            if not mm:
                continue
            base, name, arr = mm.group(1), mm.group(2), mm.group(3)
            if arr.strip() or "*" in base:
                raw[name] = "p"
            elif re.search(r"\b(struct|union)\b", base) or "{" in base:
                raw[name] = "S"
            elif re.search(r"\benum\b", base):
                raw[name] = "i"
            else:
                raw[name] = base.strip()
    out = {}

    def res(n, depth=0):
        v = raw.get(n)
        if v is None or depth > 10:
            return None
        if len(v) == 1:
            return v
        return classify_base(v, res, depth + 1)
    for n in raw:
        r = res(n)
        if r:
            out[n] = r
    return out


def classify_base(s, res, depth=0):
    """Class letter of a type without pointer declarators"""
    words = re.findall(r"\w+", s)
    words = [w for w in words if w not in ("const", "volatile", "register", "extern", "static", "inline")]
    if not words:
        return None
    if "struct" in words or "union" in words:
        return "S"
    if "enum" in words:
        return "i"
    if "long" in words and words.count("long") >= 2:
        return "l"
    if "unsigned" in words:
        if "char" in words:
            return "B"
        if "short" in words:
            return "H"
        return "I"
    for w in words:
        if w in SCALARS:
            return SCALARS[w]
        r = res(w, depth) if res else None
        if r:
            return r
    return None


def split_params(s):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def param_name(p):
    """The parameter's name, if it has one (for the cast scan)"""
    if "(" in p:
        m = re.search(r"\(\s*\*\s*(\w+)\s*\)", p)
        return m.group(1) if m else None
    m = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", p)
    if not m:
        return None
    words = re.findall(r"\w+", p)
    return m.group(1) if len(words) >= 2 and m.group(1) not in SCALARS else None


def classify_param(p, tds):
    p = p.strip()
    if p == "...":
        return "."
    if "*" in p or "[" in p or "(" in p:
        return "p"
    return classify_base(p, lambda n, d=0: tds.get(n))


def signature(ret, params, tds):
    r = classify_param(ret, tds) if ret.strip() else "i"
    ps = []
    plist = split_params(params)
    if plist in ([], ["void"]):
        plist = []
    for p in plist:
        c = classify_param(p, tds)
        if c is None or c == "v":
            return None
        ps.append(c)
    if r is None:
        return None
    return r + ":" + "".join(ps)


# ------------------------------------------------------------------ prototypes
PROTO_RE = r"(?m)(?:^|(?<=[;}}\n]))[ \t]*((?:[\w\*]+[ \t\*]+)+?)\**\s*\b{name}\s*\(((?:[^;{{}}()]|\([^;{{}}()]*\)|\([^;{{}}()]*\([^;{{}}()]*\)[^;{{}}()]*\))*)\)\s*([;{{])"


def find_protos(names, files):
    """name -> [(kind, file, ret, params, body)]; kind 'def' or 'decl'"""
    want = set(names)
    out = collections.defaultdict(list)
    for path in files:
        t = read(path)
        if not any(n in t for n in want):
            continue
        rel = os.path.relpath(path, ROOT).replace("\\", "/")
        for m in re.finditer(PROTO_RE.format(name=r"(\w+)"), t):
            name = m.group(2)
            if name not in want:
                continue
            ret = m.group(1).strip()
            if set(re.findall(r"\w+", ret)) & KEYWORDS:
                continue
            if re.search(r"\btypedef\b", ret):
                continue
            body = ""
            if m.group(4) == "{":
                depth, i = 1, m.end()
                while i < len(t) and depth:
                    depth += {"{": 1, "}": -1}.get(t[i], 0)
                    i += 1
                body = t[m.end():i]
            out[name].append(("def" if m.group(4) == "{" else "decl", rel, ret, m.group(3), body))
    return out


def casts_to_pointer(body, pname):
    """Does the body cast the parameter to a pointer type?"""
    if not pname:
        return False
    n = re.escape(pname)
    # the cast applies to the bare name, not to an expression it starts
    bare = r"\s*(?:" + n + r"|\(\s*" + n + r"\s*\))\s*[);,\]]"
    return bool(re.search(r"\(\s*(?:const\s+)?\w+(?:\s+\w+)?\s*\*+\s*\)" + bare, body) or
                re.search(r"\(\s*\w+\s*\(\s*\*\s*\)\s*\([^()]*\)\s*\)" + bare, body))


# name -> parameter classes of every function declared in include/protos/
# (for passes_to_pointer)
PROTO_CLASSES = {}


def call_args(body):
    """(callee, [argument text]) for each direct call in a function body"""
    for m in re.finditer(r"\b(\w+)\s*\(", body):
        i, depth, cur, args = m.end(), 1, "", []
        while i < len(body) and depth:
            ch = body[i]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth == 0:
                    break
            if ch == "," and depth == 1:
                args.append(cur.strip())
                cur = ""
            else:
                cur += ch
            i += 1
        args.append(cur.strip())
        yield m.group(1), args


def passes_to_pointer(body, pname):
    """Does the body hand the parameter, as it is, to a pointer parameter
    of a function its prototype declares (FxAtkHitRegister's arg1 goes to
    BtlHitQueueAdd's BtlUnit *)?"""
    if not pname:
        return None
    bare = re.compile(r"(?:\(\s*[\w\s\*]+\)\s*)?" + re.escape(pname))
    for callee, args in call_args(body):
        cls = PROTO_CLASSES.get(callee)
        if not cls:
            continue
        for k, a in enumerate(args):
            if k < len(cls) and cls[k] == "p" and bare.fullmatch(a):
                return callee
    return None


def choose(name, cands, tds, notes):
    if name in OVERRIDES:
        return OVERRIDES[name], "override"
    defs = [c for c in cands if c[0] == "def"]
    hdrs = [c for c in cands if c[0] == "decl" and c[1].startswith("include/")]
    hdrs.sort(key=lambda c: (not c[1].startswith("include/protos/"), c[1]))
    decls = [c for c in cands if c[0] == "decl" and not c[1].startswith("include/")]
    sigs = collections.Counter()
    for c in cands:
        s = signature(c[2], c[3], tds)
        if s:
            sigs[s] += 1
    pick = None
    for group, how in ((defs, "definition"), (hdrs, "header")):
        for c in group:
            s = signature(c[2], c[3], tds)
            if s:
                pick = (s, how, c)
                break
        if pick:
            break
    if not pick and sigs:
        s = sigs.most_common(1)[0][0]
        c = next(c for c in decls if signature(c[2], c[3], tds) == s)
        pick = (s, "declarations", c)
    if not pick:
        return None, None
    s, how, c = pick
    # an int parameter the definition uses as a pointer
    if defs:
        d = next((x for x in defs if signature(x[2], x[3], tds) == s), None)
        if d:
            plist = [p for p in split_params(d[3]) if p != "void"]
            ret, ps = s.split(":")
            ps = list(ps)
            for k, p in enumerate(plist):
                if k < len(ps) and ps[k] in "iI" and casts_to_pointer(d[4], param_name(p)):
                    ps[k] = "p"
                    notes.append(f"{name}: parameter {k + 1} ({p}) is cast to a pointer in {d[1]}: a pointer")
                elif k < len(ps) and ps[k] in "iI":
                    callee = passes_to_pointer(d[4], param_name(p))
                    if callee:
                        ps[k] = "p"
                        notes.append(f"{name}: parameter {k + 1} ({p}) goes to a pointer parameter of {callee}: a pointer")
            s = ret + ":" + "".join(ps)
    # classes that disagree between the candidates (ints vs floats)
    def shape(x):
        return re.sub(r"[bBhHiIp]", "i", x)
    others = {shape(x) for x in sigs} - {shape(s)}
    if others:
        notes.append(f"{name}: {s} from the {how} ({c[1]}); other declarations say {', '.join(sorted(others))}")
    return s, how


# ------------------------------------------------------------------ globals
class Objects:
    def __init__(self):
        self.objs = []  # (start, end, name)
        for rel in ELFS:
            e = elf32.Elf(os.path.join(ROOT, rel))
            for s in e.symbols():
                if (s["name"] and s["shndx"] != elf32.SHN_UNDEF and s["size"] and s["type"] == 1
                        and not s["name"].startswith((".", "__"))):
                    self.objs.append((s["value"], s["value"] + s["size"], s["name"]))
        self.objs = sorted(set(self.objs))

    def holding(self, a):
        best = None
        for s, e, n in self.objs:
            if s > a:
                break
            if s <= a < e and (best is None or e - s < best[1] - best[0]):
                best = (s, e, n)
        return best

    def between(self, lo, hi):
        return [o for o in self.objs if lo <= o[0] and o[1] <= hi]


def global_objects(datas, notes):
    objs = Objects()
    picked = {}
    for a, name in datas:
        o = objs.holding(a)
        if o is None:
            sys.exit(f"{name} (0x{a:08X}): no object holds it in the matching build's ELFs")
        picked[o[0]] = o
    chosen = sorted(picked.values())
    fill = []
    for x, y in zip(chosen, chosen[1:]):
        if 0 < y[0] - x[1] < GAP_FILL:
            for o in objs.between(x[1], y[0]):
                fill.append(o)
                notes.append(f"global {o[2]} (0x{o[0]:08X}, 0x{o[1] - o[0]:X} bytes): between {x[2]} and {y[2]}")
    out = sorted(set(chosen) | set(fill))
    # an object inside another (a sized alias) is left to the outer one
    res = []
    for o in out:
        if res and o[0] < res[-1][1]:
            if o[1] > res[-1][1]:
                sys.exit(f"objects overlap: {res[-1]} and {o}")
            continue
        res.append(o)
    return res


# ------------------------------------------------------------------ output
HEAD = """/* Generated by port/tools/gen_mips_bridge.py from config/abi_main.txt and
   config/abi_battle.txt ({what}). Do not edit: rerun the script. */
"""


def write_abi(path, funcs, globs):
    lines = [HEAD.format(what="the tables, no game symbols"), '#include "grx_mips.h"', "",
             "const GrxMipsAbiFunc grx_mips_abi_funcs[] = {"]
    for a, name, sig in funcs:
        lines.append(f'    {{0x{a:08X}u, "{name}", "{sig}"}},')
    lines += ["};", "const int grx_mips_abi_nfuncs = sizeof(grx_mips_abi_funcs) / sizeof(grx_mips_abi_funcs[0]);",
              "", "const GrxMipsAbiData grx_mips_abi_data[] = {"]
    for s, e, n in globs:
        lines.append(f'    {{0x{s:08X}u, 0x{e - s:X}u, "{n}"}},')
    lines += ["};", "const int grx_mips_abi_ndata = sizeof(grx_mips_abi_data) / sizeof(grx_mips_abi_data[0]);", ""]
    return "\n".join(lines)


def call_expr(name, sig):
    ret, ps = sig.split(":")
    args, ni, nf = [], 0, 0
    for c in ps:
        if c == ".":
            # varargs: the rest of the integer registers (the EABI passes
            # variadic arguments in GPRs, floats promoted to soft doubles)
            while ni < 8:
                args.append(f"grx_mips_arg_v(c, {ni})")
                ni += 1
            break
        if c == "f":
            args.append(f"grx_mips_arg_f(c, {nf})")
            nf += 1
        elif c == "p":
            args.append(f"grx_mips_arg_p(c, {ni})")
            ni += 1
        elif c in "dl":
            args.append(f"({C_TYPES[c]})grx_mips_arg_{c}(c, {ni})")
            ni += 1
        else:
            args.append(f"({C_TYPES[c]})grx_mips_arg_i(c, {ni})")
            ni += 1
    if ni > 8 or nf > 8:
        sys.exit(f"{name}: more arguments than the EABI's registers ({sig})")
    call = f"{name}({', '.join(args)})"
    if ret == "v":
        return call + ";"
    if ret == "f":
        return f"grx_mips_ret_f(c, {call});"
    if ret == "p":
        return f"grx_mips_ret_p(c, {call});"
    if ret == "d":
        return f"grx_mips_ret_d(c, {call});"
    if ret == "l":
        return f"grx_mips_ret_l(c, {call});"
    return f"grx_mips_ret_i(c, ({C_TYPES[ret]}){call});"


def decl(name, sig):
    ret, ps = sig.split(":")
    plist = [("..." if c == "." else C_TYPES[c]) for c in ps] or ["void"]
    r = C_TYPES[ret]
    return f"{r}{'' if r.endswith('*') else ' '}{name}({', '.join(plist)});"


def write_bridge(path, funcs, globs):
    libs = sorted({LIBC[n] for _, n, _ in funcs if n in LIBC})
    lines = [HEAD.format(what="the bridge to the port's functions and globals"),
             "#include <stdint.h>"] + [f"#include <{h}>" for h in libs] + ['#include "grx_mips.h"', "",
             "/* The functions, declared with the classes the EABI passes their",
             "   arguments in (pointers as void *); none of the game's headers is",
             "   included, so these are the only declarations here */"]
    for _, n, sig in funcs:
        if n in LIBC or n in BUILTINS:
            continue
        lines.append(decl(n, sig))
    lines.append("")
    for g in globs:
        lines.append(f"extern unsigned char {g[2]}[];")
    lines.append("")
    hnames = []
    for k, (_, n, sig) in enumerate(funcs):
        if n in BUILTINS:
            hnames.append(BUILTINS[n])
            continue
        h = f"br_{n}"
        hnames.append(h)
        lines.append(f"static void {h}(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {{")
        lines.append("    (void)f;")
        if sig.split(":")[0] == "v" and sig.endswith(":"):
            lines.append("    (void)c;")
        lines.append(f"    {call_expr(n, sig)}")
        lines.append("}")
    lines += ["", "static const GrxMipsHandler handlers[] = {"]
    lines += [f"    {h}," for h in hnames]
    lines += ["};", "", "static void *const func_hosts[] = {"]
    for _, n, _ in funcs:
        lines.append("    0," if n in BUILTINS else f"    (void *)(uintptr_t){n},")
    lines += ["};", "", "static void *const data_hosts[] = {"]
    lines += [f"    {g[2]}," for g in globs]
    lines += ["};", "",
              "void grx_mips_install_bridge(void) {",
              "    grx_mips_set_bridge(handlers, func_hosts, data_hosts);",
              "}", ""]
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--check", action="store_true", help="report only")
    ap.add_argument("-v", action="store_true", help="list every signature")
    args = ap.parse_args()

    funcs, datas = abi_lists()
    headers = glob.glob(os.path.join(ROOT, "include", "**", "*.h"), recursive=True)
    sources = (glob.glob(os.path.join(ROOT, "src", "**", "*.c"), recursive=True)
               + glob.glob(os.path.join(ROOT, "port", "game", "**", "*.c"), recursive=True))
    tds = typedefs([read(p) for p in headers])
    for h in glob.glob(os.path.join(ROOT, "include", "protos", "*.h")):
        for m in re.finditer(PROTO_RE.format(name=r"(\w+)"), read(h)):
            sig = signature(m.group(1), m.group(3), tds)
            if sig and m.group(4) == ";":
                PROTO_CLASSES.setdefault(m.group(2), sig.split(":")[1])
    cands = find_protos([n for _, n in funcs], headers + sources)
    notes, out, missing = [], [], []
    hows = collections.Counter()
    for a, n in funcs:
        sig, how = choose(n, cands.get(n, []), tds, notes)
        if sig is None:
            missing.append(n)
            continue
        if "S" in sig:
            sys.exit(f"{n}: a struct passed or returned by value ({sig}); add it to OVERRIDES")
        hows[how] += 1
        out.append((a, n, sig))
    if missing:
        sys.exit("no prototype for " + ", ".join(missing) + ": add them to OVERRIDES")
    globs = global_objects(datas, notes)

    print(f"{len(out)} functions ({', '.join(f'{v} from the {k}' for k, v in sorted(hows.items()))}), "
          f"{len(globs)} global objects")
    for n in notes:
        print("  note:", n)
    if args.v:
        for a, n, s in out:
            print(f"  {a:08X} {s:12} {n}")
        for s, e, n in globs:
            print(f"  {s:08X} {e - s:6X} {n}")
    if args.check:
        return
    for rel, text in ((OUT_ABI, write_abi(OUT_ABI, out, globs)), (OUT_BRIDGE, write_bridge(OUT_BRIDGE, out, globs))):
        path = os.path.join(ROOT, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", newline="\n") as f:
            f.write(text)
        print("wrote", rel)


if __name__ == "__main__":
    main()
