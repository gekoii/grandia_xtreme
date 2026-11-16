#!/usr/bin/env python3
"""Find calls whose declaration passes arguments differently on i686 than
the function's definition expects (port/README.md).

    python port/tools/proto_audit.py            # writes build/port/proto_audit.txt
    python port/tools/proto_audit.py -j 8 --verbose

The game was built for the EE's EABI, where float arguments go in $f12...
and everything else in $a0-$t3, independently. A declaration that moves a
float past an int, or drops a trailing float, or says float for an int
return, still produced byte-identical code there. On i686 (cdecl, every
argument on the stack in order, float returns in st(0)) it passes the wrong
values. The decompilation is full of such declarations: local prototypes
in the calling file, casts at the call (`((T (*)(...))f)(...)`), implicit
declarations, and the odd wrong header prototype.

How: every game source (the port's generated copy, made afresh in a
temporary directory, compiled with build.game_flags()), every port/game/
source, the host-replaced library sources and port/src/ are run through
`clang -fsyntax-only -Xclang -ast-dump`. From the text dump come every
function declaration and definition (with the typedefs resolved, each
parameter is classed: i = int or pointer, f = float, d = double, q = 64-bit
integer, S = struct by value...) and every direct call or call through a
cast of a function name, with the classes of the arguments it passes.

Each call is then checked against the callee's definition (a C definition
in src/, port/game/, a library source or the platform layer): the callee
reads its parameters at the offsets its definition says, so a call is
*harmful* when the classes it pushes are not those, in that order, or it
pushes too few, or one side returns in st(0) and the other doesn't, or the
caller reads a result the callee never produced. Every declaration and cast
is also compared with the definition statically, whether or not anything
calls it. Functions with no C definition anywhere (still in assembly, or
the C runtime's) have their declarations compared with each other.

The fix for a harmful call is worked out the way the EE passed it: the
i-th integer-class argument went to the i-th integer-class parameter and
the i-th float to the i-th float, so that is what the call should pass on
i686 too. A parameter the call didn't pass at all got whatever was in its
register, most often the caller's own parameter in the same register; that
is suggested, marked "probably".

Not covered: calls through function pointers held in variables, tables or
struct fields (their targets aren't known from the source), and macro
call sites (their location is the macro's definition, not its use).
"""

import argparse
import collections
import glob
import multiprocessing
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import build  # noqa: E402

ROOT = build.ROOT
REPORT = os.path.join(build.OUT, "proto_audit.txt")

# ---------------------------------------------------------------------------
# Types


INT_WORDS = {"char", "short", "int", "long", "signed", "unsigned", "_Bool", "__int8", "__int16", "__int32",
             # clang's predefined sugar for size_t and friends
             "__size_t", "__signed_size_t", "__ptrdiff_t", "__intptr_t", "__uintptr_t", "__wchar_t"}
QUALS = re.compile(r"\b(const|volatile|restrict|__restrict|__restrict__|register|__unaligned)\b")
ANON = re.compile(r"\((?:unnamed|anonymous)(?: struct| union| enum)? at ([^()]*)\)")
ATTR_TAIL = re.compile(r"\s*__attribute__\(\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)\)\s*$")
FP = {"f", "d", "x"}


def classify(t, tmap, seen=0):
    """The i686 class of a parameter or return type"""
    t = QUALS.sub("", ANON.sub(r"<anon:\1>", t)).strip()
    t = re.sub(r"\s+", " ", t)
    if "vector_size" in t or "ext_vector_type" in t:
        return "V"
    if "*" in t or "[" in t or "(" in t:
        return "i"          # a pointer (an array or function parameter decays to one)
    if t.startswith(("struct ", "union ")):
        return "S" + t[t.index(" "):]
    if t.startswith("enum "):
        return "i"
    if t == "void":
        return "v"
    if t == "float":
        return "f"
    if t == "double":
        return "d"
    if t == "long double":
        return "x"
    if t in tmap and seen < 30:
        return classify(tmap[t], tmap, seen + 1)
    if "long long" in t or "__int64" in t:
        return "q"
    if "__int128" in t:
        return "o"
    if "_Complex" in t:
        return "C"
    if set(t.split()) & INT_WORDS:
        return "i"
    return "?" + t


def _last_group(t):
    """'R (P)' -> ('R', 'P'), matching the final ')'"""
    t = t.rstrip()
    if not t.endswith(")"):
        return None
    depth = 0
    for i in range(len(t) - 1, -1, -1):
        c = t[i]
        if c == ")":
            depth += 1
        elif c == "(":
            depth -= 1
            if depth == 0:
                return t[:i].rstrip(), t[i + 1:-1]
    return None


def _split_params(p):
    out, depth, cur = [], 0, []
    for c in p:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if c == "," and depth == 0:
            out.append("".join(cur).strip())
            cur = []
        else:
            cur.append(c)
    if "".join(cur).strip():
        out.append("".join(cur).strip())
    return out


def parse_fn(t, tmap):
    """A function type (or pointer to one) -> (ret, params, variadic, noproto), or None"""
    t = ANON.sub(r"<anon:\1>", t).strip()
    while True:
        m = ATTR_TAIL.search(t)
        if not m:
            break
        t = t[:m.start()]
    g = _last_group(t)
    if g is None:
        return None
    ret, params = g
    ret_override = None
    for _ in range(4):
        if not ret.endswith(")"):
            break
        g2 = _last_group(ret)
        if g2 is None:
            return None
        base, inner = g2
        inner = QUALS.sub("", inner).strip()
        if "(" not in inner:            # 'R (*)(P)': a pointer to the function
            ret = base
            continue
        # 'R (*(P))(Q)': a function of P returning a pointer to a function
        g3 = _last_group(inner)
        if g3 is None:
            return None
        params = g3[1]
        ret, ret_override = "void *", "i"
        break
    ps = _split_params(params)
    noproto = not ps
    variadic = bool(ps) and ps[-1] == "..."
    if variadic:
        ps = ps[:-1]
    if ps == ["void"]:
        ps = []
    return (ret_override or classify(ret, tmap), tuple(classify(p, tmap) for p in ps), variadic, noproto)


# ---------------------------------------------------------------------------
# The AST dump

PREFIX = re.compile(r"[ |`-]*")
LOC = re.compile(r"<invalid sloc>|line:(\d+):\d+|col:\d+|<(?:built-in|scratch space|command line)>:(\d+):\d+"
                 r"|([^\s<>,'=]+):(\d+):\d+(?=[\s>,]|$)")
FUNC_REF = re.compile(r" Function (0x[0-9a-f]+) '([^']+)'")
CHAIN = {"ImplicitCastExpr", "ParenExpr", "CStyleCastExpr", "UnaryOperator"}
UNUSED_PARENTS = {"CompoundStmt", "LabelStmt", "DefaultStmt"}
# children of these that are statements (a call there is a statement; its result is unused)
STMT_CHILDREN = {"IfStmt": (1, 2), "WhileStmt": (1,), "ForStmt": (0, 3, 4), "DoStmt": (0,), "CaseStmt": (1,)}


def _types(line, q):
    """The (sugared, desugared) type quoted at q"""
    e = line.index("'", q + 1)
    t = line[q + 1:e]
    if line.startswith(":'", e + 1):
        e2 = line.index("'", e + 3)
        return t, line[e + 3:e2]
    return t, t


def norm_path(p):
    p = p.replace("\\", "/")
    r = os.path.normpath(p).replace("\\", "/")
    if os.path.isabs(r):
        try:
            rr = os.path.relpath(r, ROOT).replace("\\", "/")
            if not rr.startswith(".."):
                return rr
        except ValueError:
            pass
        # a toolchain header: keep the part from include/ on
        i = r.rfind("/include/")
        return "<sys>" + r[i + 8:] if i >= 0 else r
    return r


def parse_dump(text):
    """(decls by address, decls, calls, typedefs) from one translation unit's
    text AST dump. Locations in the dump are relative to the one printed
    before (file, then line, then column), so every location is followed."""
    tmap = {}
    sig_cache = {}
    cls_cache = {}

    def fsig(t):
        s = sig_cache.get(t)
        if s is None:
            s = sig_cache[t] = parse_fn(t, tmap)
        return s

    def cls(t):
        c = cls_cache.get(t)
        if c is None:
            c = cls_cache[t] = classify(t, tmap)
        return c

    by_addr = {}        # addr -> decl dict
    decls = []
    calls = []
    stack = []          # per depth: (kind, decl or None)
    open_calls = []
    cur_file, cur_line = "?", 0
    top_fn = None

    def finish(c):
        if c["callee"] is None or c["ctype"] is None:
            return
        name, addr = c["callee"]
        if name.startswith("__builtin"):
            return
        calls.append(c)

    for raw in text.splitlines():
        p = PREFIX.match(raw).end()
        if p >= len(raw):
            continue
        depth = p // 2
        sp = raw.find(" ", p)
        kind = raw[p:sp] if sp > 0 else raw[p:]
        q = raw.find("'", p)
        head = raw[p:q] if q >= 0 else raw[p:]
        first = None
        if ":" in head:
            for m in LOC.finditer(head):
                g = m.group(0)
                if g.startswith("line:"):
                    cur_line = int(m.group(1))
                elif g.startswith("col:") or g == "<invalid sloc>":
                    pass
                elif m.group(2) is not None:
                    cur_file, cur_line = g[:g.index(">") + 1], int(m.group(2))
                else:
                    cur_file, cur_line = m.group(3), int(m.group(4))
                if first is None and g != "<invalid sloc>":
                    first = (cur_file, cur_line, int(g.rsplit(":", 1)[1]))
        del stack[depth:]
        while len(stack) < depth:
            stack.append(["?", None, 0])
        idx = -1
        if depth:
            idx = stack[depth - 1][2]
            stack[depth - 1][2] += 1
        stack.append([kind, None, 0])
        while open_calls and open_calls[-1]["depth"] >= depth:
            finish(open_calls.pop())

        if open_calls:
            c = open_calls[-1]
            if depth == c["depth"] + 1:
                c["n"] += 1
                if c["n"] == 1:
                    if q >= 0:
                        c["ctype"] = _types(raw, q)[1]
                    c["chain"] = depth if kind in CHAIN or kind == "DeclRefExpr" else None
                    if kind == "CStyleCastExpr":
                        c["cast"] = True
                    if kind == "DeclRefExpr":
                        m = FUNC_REF.search(raw)
                        if m:
                            c["callee"] = (m.group(2), m.group(1))
                        c["chain"] = None
                elif q >= 0:
                    c["args"].append(cls(_types(raw, q)[1]))
                else:
                    c["args"].append("?")
            elif c["n"] == 1 and c["chain"] is not None:
                if depth == c["chain"] + 1 and (kind in CHAIN or kind == "DeclRefExpr"):
                    c["chain"] = depth
                    if kind == "CStyleCastExpr":
                        c["cast"] = True
                    if kind == "DeclRefExpr":
                        m = FUNC_REF.search(raw)
                        if m:
                            c["callee"] = (m.group(2), m.group(1))
                        c["chain"] = None
                else:
                    c["chain"] = None

        if kind == "CallExpr" and q >= 0:
            parent = stack[depth - 1][0] if depth else "?"
            used = not (parent in UNUSED_PARENTS or stack[depth - 1][1] == "tovoid"
                        or idx in STMT_CHILDREN.get(parent, ()))
            open_calls.append({"depth": depth, "n": 0, "ctype": None, "chain": None, "cast": False,
                               "callee": None, "args": [], "ret": cls(_types(raw, q)[1]), "used": used,
                               "loc": first or (cur_file, cur_line, 0),
                               "func": top_fn["name"] if top_fn else None})
        elif kind == "CStyleCastExpr" and "<ToVoid>" in raw:
            stack[-1][1] = "tovoid"
        elif kind == "FunctionDecl" and q >= 0:
            sug, des = _types(raw, q)
            name = head.split()[-1]
            addr = raw[sp + 1:].split()[0]
            rest = raw[q:]
            d = {"name": name, "file": cur_file, "line": cur_line, "type": sug, "sig": fsig(des),
                 "internal": "internal-linkage" in rest or " static" in rest, "block": depth > 1,
                 "def": False, "params": [], "addr": addr}
            by_addr[addr] = d
            decls.append(d)
            stack[-1][1] = d
            if depth == 1:
                top_fn = d
        elif kind == "ParmVarDecl" and depth >= 2 and stack[depth - 1][0] == "FunctionDecl":
            fd = stack[depth - 1][1]
            if q >= 0 and fd is not None:
                w = head.split()
                pname = w[-1] if w and not re.match(r"(col|line):|0x|<", w[-1]) and w[-1] not in ("used", "referenced") and ":" not in w[-1] else ""
                fd["params"].append((pname, cls(_types(raw, q)[1]), _types(raw, q)[0]))
        elif kind == "CompoundStmt" and depth >= 1 and stack[depth - 1][0] == "FunctionDecl":
            fd = stack[depth - 1][1]
            if fd is not None:
                fd["def"] = True
        elif kind == "TypedefDecl" and q >= 0:
            w = head.split()
            if w:
                tmap[w[-1]] = _types(raw, q)[1]
                sig_cache.clear()
                cls_cache.clear()
    while open_calls:
        finish(open_calls.pop())
    return by_addr, decls, calls, tmap


# ---------------------------------------------------------------------------
# Workers

_TMP = None
_SEEN = set()
_CLANG = None
_SDL_INC = []


def _init(tmp, clang, sdl_inc):
    global _TMP, _CLANG, _SDL_INC
    _TMP, _CLANG, _SDL_INC = tmp, clang, sdl_inc


def sdl_include():
    """SDL3's include flags for the platform layer, as build.py finds them
    (none if it isn't there: the files that include it are then rejected)"""
    try:
        return ["-I", os.path.join(build.find_sdl3(), "include")]
    except SystemExit:
        return []


CRT_HEADERS = ["stdlib.h", "stdio.h", "string.h", "math.h", "ctype.h", "time.h", "setjmp.h", "stdarg.h",
               "wchar.h", "locale.h", "signal.h", "errno.h", "malloc.h"]


def _struct_sizes(src, flags, classes, tmap):
    """{struct class: size} for the struct types passed or returned by value,
    from a copy of the source with a sizeof() probe for each appended"""
    spell = {}
    for c in classes:
        t = c[1:].strip()
        if "<anon:" not in t:
            spell[c] = t
        else:
            for n, v in tmap.items():
                if classify(v, tmap) == c:
                    spell[c] = n
                    break
    if not spell:
        return {}
    order = sorted(spell)
    with open(src, encoding="latin-1") as f:
        text = f.read()
    probe = src[:-2] + "_sizes.c"
    with open(probe, "w", encoding="latin-1", newline="\n") as f:
        f.write(text + "\n" + "".join(f"typedef char __grx_pa_sz_{k}[sizeof({spell[c]})];\n"
                                      for k, c in enumerate(order)))
    p = subprocess.run([_CLANG, "-target", build.TARGET, "-fsyntax-only", probe] + flags
                       + ["-Xclang", "-ast-dump", "-Xclang", "-ast-dump-filter=__grx_pa_sz", "-fno-color-diagnostics"],
                       capture_output=True, cwd=ROOT)
    os.remove(probe)
    out = {}
    for m in re.finditer(r"__grx_pa_sz_(\d+) 'char\[(\d+)\]'", p.stdout.decode("latin-1")):
        out[order[int(m.group(1))]] = int(m.group(2))
    return out


def dump_one(job):
    """One translation unit -> (rel, error, decls, calls), all plain data"""
    rel, kind, binary, opt = job
    try:
        if kind == "platform":
            src = os.path.join(ROOT, rel)
            # as build.py compiles the platform layer: the game's headers
            # (kernel.c includes sdk/eekernel.h) and SDL3's
            flags = ["-I", "port/include", "-I", "include"] + _SDL_INC + ["-g", f"-O{opt}"] + build.PORT_FLAGS
        elif kind == "crt":
            # the C runtime's own prototypes, for what the host provides
            src = os.path.join(_TMP, "crt_protos.c")
            with open(src, "w") as f:
                f.write("".join(f"#include <{h}>\n" for h in CRT_HEADERS))
            flags = ["-g", f"-O{opt}"] + build.PORT_FLAGS
        else:
            src = build.generate(rel, out_dir=_TMP)
            flags = build.game_flags(rel, opt)
    except Exception as e:  # noqa: BLE001
        return rel, f"generate: {e}", [], []
    p = subprocess.run([_CLANG, "-target", build.TARGET, "-fsyntax-only", src] + flags
                       + ["-Xclang", "-ast-dump", "-fno-color-diagnostics"],
                       capture_output=True, cwd=ROOT)
    err = None
    if p.returncode:
        err = p.stderr.decode("latin-1")[-2000:] or "failed"
    by_addr, decls, calls, tmap = parse_dump(p.stdout.decode("latin-1"))
    me = rel
    out_decls = []
    for d in decls:
        f = norm_path(d["file"])
        d["file"] = f
        if kind == "crt" and not d["internal"] and not d["block"]:
            d["def"] = True
        key = (d["name"], f, d["line"])
        if d["internal"]:
            key += (me,)
        d["key"] = key
        if d["def"] or f == me or d["internal"]:
            out_decls.append(d)
        elif key not in _SEEN:
            _SEEN.add(key)
            out_decls.append(d)
    out_calls = []
    for c in calls:
        name, addr = c["callee"]
        d = by_addr.get(addr)
        f, line, col = c["loc"]
        out_calls.append({
            "name": name, "decl": d["key"] if d else None, "internal": bool(d and d["internal"]),
            "decl_sig": d["sig"] if d else None, "cast": c["cast"],
            "ctype": c["ctype"], "args": tuple(c["args"]), "ret": c["ret"], "used": c["used"],
            "file": norm_path(f), "line": line, "col": col, "func": c["func"], "tu": me,
        })
    for d in out_decls:
        d.pop("addr", None)

    # Structs by value: what matters on i686 is their size (a 4- or 8-byte
    # one is passed and returned like an int or an s64)
    def sig_classes(sg):
        return [] if sg is None else [sg[0], *sg[1]]
    classes = set()
    for d in out_decls:
        classes.update(sig_classes(d["sig"]))
        classes.update(pc for _, pc, _ in d["params"])
    for c in out_calls:
        classes.update(c["args"])
        classes.add(c["ret"])
        classes.update(sig_classes(c["decl_sig"]))
    classes = {c for c in classes if c.startswith("S ")}
    if classes:
        sizes = _struct_sizes(src, flags, classes, tmap)

        def rc(c):
            if c.startswith("S "):
                return f"S{sizes.get(c, '?')}:{c[2:]}"
            return c

        def rs(sg):
            return None if sg is None else (rc(sg[0]), tuple(rc(x) for x in sg[1]), sg[2], sg[3])
        for d in out_decls:
            d["sig"] = rs(d["sig"])
            d["params"] = [(n, rc(pc), t) for n, pc, t in d["params"]]
        for c in out_calls:
            c["args"] = tuple(rc(x) for x in c["args"])
            c["ret"] = rc(c["ret"])
            c["decl_sig"] = rs(c["decl_sig"])
    return rel, err, out_decls, out_calls


# ---------------------------------------------------------------------------
# Comparing

NAMES = {"i": "int", "f": "f32", "d": "f64", "q": "s64", "v": "void", "x": "long double", "V": "vector", "o": "s128", "C": "complex"}
GPR = lambda c: c != "f"  # noqa: E731  (on the EE: only single floats go in $f12...)


def cname(c):
    if c.startswith("S"):
        return c[c.index(":") + 1:] if ":" in c else f"struct ({c[1:]} bytes)"
    return NAMES.get(c, c)


def sig_str(s):
    if s is None:
        return "?"
    ret, ps, var, noproto = s
    if noproto:
        inner = ""
    else:
        inner = ", ".join(cname(p) for p in ps) + (", ..." if var else "") if (ps or var) else "void"
    return f"{cname(ret)} ({inner})"


def norm(c, ret=False):
    """The i686 class: a struct of 4 bytes or less goes like an int, one of 8 like an s64"""
    if c.startswith("S"):
        size = c[1:c.index(":")]
        if size.isdigit():
            n = int(size)
            if n <= 4 and (not ret or n in (1, 2, 4)):
                return "i"
            if n == 8:
                return "q"
            return f"S{n}"
    return c


def same(a, b):
    return a == b or norm(a) == norm(b)


def nsig(s):
    return None if s is None else (norm(s[0], True), tuple(norm(x) for x in s[1]), s[2], s[3])


KIND_ORDER = ["param-order", "param-count", "param-64", "param-float-double", "struct-by-value", "param-class",
              "variadic", "no-prototype", "return-class", "return-64", "return-struct", "return-void",
              "return-float-double"]


def compare(s, ref):
    """Kinds of difference between declaration s and definition ref that matter on i686"""
    kinds = []
    r1, p1, v1, n1 = nsig(s)
    r2, p2, v2, n2 = nsig(ref)
    if n1 and not n2:
        if any(c in ("f",) for c in p2) or v2:
            kinds.append("no-prototype")
    elif not (n1 or n2):
        if len(p1) != len(p2):
            kinds.append("param-count")
        common = min(len(p1), len(p2))
        if p1[:common] != p2[:common]:
            def pre(a, b):
                n = min(len(a), len(b))
                return a[:n] == b[:n]
            # the same values in the same registers on the EE, in other slots on i686
            if pre([c for c in p1 if GPR(c)], [c for c in p2 if GPR(c)]) and                     pre([c for c in p1 if c == "f"], [c for c in p2 if c == "f"]):
                kinds.append("param-order")
            for a, b in zip(p1, p2):
                pair = {a, b}
                if a == b or pair == {"i", "f"}:
                    continue
                if pair == {"i", "q"}:
                    kinds.append("param-64")
                elif pair == {"f", "d"}:
                    kinds.append("param-float-double")
                elif a.startswith("S") or b.startswith("S"):
                    kinds.append("struct-by-value")
                else:
                    kinds.append("param-class")
            if "param-order" not in kinds and any({a, b} == {"i", "f"} for a, b in zip(p1, p2)):
                kinds.append("param-class")
        if v1 != v2:
            kinds.append("variadic")
    if r1 != r2:
        pair = {r1, r2}
        if pair <= FP:
            kinds.append("return-float-double")
        elif "v" in pair and not (pair & FP):
            kinds.append("return-void")
        elif pair & FP:
            kinds.append("return-class")
        elif pair == {"i", "q"}:
            kinds.append("return-64")
        elif r1.startswith("S") or r2.startswith("S"):
            kinds.append("return-struct")
        else:
            kinds.append("return-class")
    seen = []
    for k in kinds:
        if k not in seen:
            seen.append(k)
    return sorted(seen, key=KIND_ORDER.index)


def harm(call, ref):
    """Why this call passes or reads the wrong thing on i686, against the callee's definition ref"""
    why = []
    ret, ps, var, noproto = nsig(ref)
    args = tuple(norm(a) for a in call["args"])
    if not noproto:
        if len(args) < len(ps):
            why.append(f"passes {len(args)} of {len(ps)} parameters")
        bad = [k for k, (a, b) in enumerate(zip(args, ps)) if a != b]
        if bad:
            why.append("args " + ", ".join(f"{k + 1}: {cname(args[k])} read as {cname(ps[k])}" for k in bad))
    r = norm(call["ret"], True)
    if r != ret:
        if (r in FP) != (ret in FP):
            if r in FP or ret in FP:
                why.append(f"returns {cname(ret)}, read as {cname(r)} (st(0) vs eax"
                           + (", x87 stack unbalanced)" if (r == "v" or ret == "v" or not call["used"]) else ")"))
        elif r == "v" or not call["used"]:
            pass
        elif ret == "v":
            why.append(f"reads a {cname(r)} result from a void function")
        elif r == "q" and ret == "i":
            why.append("reads s64 from an int function (edx is garbage)")
        elif r.startswith("S") or ret.startswith("S"):
            why.append(f"returns {cname(ret)}, read as {cname(r)}")
    return why


# ---------------------------------------------------------------------------
# Call text and the fix


def _src(rel, cache={}):  # noqa: B006
    if rel not in cache:
        try:
            with open(os.path.join(ROOT, rel), encoding="latin-1") as f:
                cache[rel] = f.read()
        except OSError:
            cache[rel] = None
    return cache[rel]


def _skip_group(t, i):
    """t[i] == '(' -> index after its match (strings and comments skipped)"""
    depth = 0
    n = len(t)
    while i < n:
        c = t[i]
        if c in "\"'":
            j = i + 1
            while j < n and t[j] != c:
                j += 2 if t[j] == "\\" else 1
            i = j + 1
            continue
        if t.startswith("/*", i):
            i = t.find("*/", i) + 2
            if i < 2:
                return n
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return n


def call_args(call):
    """The argument texts of a call, from the source, or None"""
    t = _src(call["file"])
    if t is None or call["line"] < 1:
        return None
    lines = t.split("\n")
    if call["line"] > len(lines):
        return None
    pos = sum(len(x) + 1 for x in lines[:call["line"] - 1]) + max(call["col"] - 1, 0)
    if t[pos:pos + 1] == "(":
        i = _skip_group(t, pos)
    else:
        m = re.compile(r"[A-Za-z_]\w*").match(t, pos)
        if not m:
            return None
        i = m.end()
    while i < len(t) and t[i] in " \t\r\n":
        i += 1
    if t[i:i + 1] != "(":
        return None
    end = _skip_group(t, i)
    inner = t[i + 1:end - 1]
    out, depth, cur, j = [], 0, [], 0
    while j < len(inner):
        c = inner[j]
        if c in "\"'":
            k = j + 1
            while k < len(inner) and inner[k] != c:
                k += 2 if inner[k] == "\\" else 1
            cur.append(inner[j:k + 1])
            j = k + 1
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == "," and depth == 0:
            out.append(" ".join("".join(cur).split()))
            cur = []
        else:
            cur.append(c)
        j += 1
    if "".join(cur).strip():
        out.append(" ".join("".join(cur).split()))
    return out


def _convert(t, a, pc, ptype):
    """Argument text t, of class a, for a parameter of class pc"""
    if same(a, pc):
        return t
    if {norm(a), norm(pc)} & {"d", "x"} and {norm(a), norm(pc)} & {"i", "q"}:
        # soft-float doubles were passed in GPRs on the EE: the bits, not the value
        return f"<{ptype} with the bits of {t}>"
    return f"({ptype})({t})"


def fix_call(call, d, caller_params):
    """The call as the EE passed it, for the definition d"""
    ret, ps, var, noproto = d["sig"]
    args = call["args"]
    texts = call_args(call)
    if texts is None or len(texts) != len(args):
        texts = [f"<arg {k + 1}>" for k in range(len(args))]
    if noproto:
        return None
    cg = [n for n, c, _ in caller_params if GPR(c)]
    cf = [n for n, c, _ in caller_params if not GPR(c)]
    kr = call["decl_sig"] is None or (call["decl_sig"][3] and not call["cast"])

    def pinfo(k, pc):
        if k < len(d["params"]):
            return (d["params"][k][2] + " " + d["params"][k][0]).strip(), d["params"][k][2]
        return cname(pc), cname(pc)

    def missing(names, gi, fi, pc):
        if GPR(pc):
            reg = ["$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3"][gi] if gi < 8 else "the stack"
            guess = f"probably {cg[gi]}, " if gi < len(cg) and cg[gi] else ""
        else:
            reg = f"$f{12 + fi}" if fi < 8 else "the stack"
            guess = f"probably {cf[fi]}, " if fi < len(cf) and cf[fi] else ""
        return f"<missing {names}: {guess}what was in {reg}>"

    out = []
    gi = fi = 0
    if kr:
        # no prototype (implicit or old-style): the arguments in the order
        # written, once the callee is declared properly
        for k, pc in enumerate(ps):
            names, ptype = pinfo(k, pc)
            if k < len(args):
                out.append(texts[k] if norm(args[k]) in ("d",) and pc == "f" else _convert(texts[k], args[k], pc, ptype))
            else:
                out.append(missing(names, gi, fi, pc))
            gi += GPR(pc)
            fi += not GPR(pc)
        extra = list(zip(texts, args))[len(ps):]
        note = f"  [declare it as {d['type']}]"
    else:
        gq = [(t, a) for t, a in zip(texts, args) if GPR(a)]
        fq = [(t, a) for t, a in zip(texts, args) if not GPR(a)]
        for k, pc in enumerate(ps):
            names, ptype = pinfo(k, pc)
            if GPR(pc):
                out.append(_convert(*gq[gi], pc, ptype) if gi < len(gq) else missing(names, gi, fi, pc))
                gi += 1
            else:
                out.append(_convert(*fq[fi], pc, ptype) if fi < len(fq) else missing(names, gi, fi, pc))
                fi += 1
        extra = gq[gi:] + fq[fi:]
        note = ""
    if extra and var:
        out += [t for t, _ in extra]
        extra = []
    s = f"{call['name']}({', '.join(out)})"
    if extra:
        s += "  [drops " + ", ".join(t for t, _ in extra) + "]"
    return s + note


# ---------------------------------------------------------------------------
# Main


def jobs(opt):
    out = []
    for rel, binary, host in build.game_sources():
        kind = "lib" if host else ("port" if rel.startswith("port/") else "game")
        out.append((rel, kind, binary, opt))
    # the platform layer with its subfolders (port/src/gs/, ...), as build.py has it
    for p in sorted(glob.glob(os.path.join(ROOT, "port", "src", "**", "*.c"), recursive=True)):
        out.append((os.path.relpath(p, ROOT).replace("\\", "/"), "platform", "main", opt))
    out.append(("<crt>", "crt", "main", opt))
    return out


def linked_libs():
    """The host-replaced library sources the last full build linked after all
    (build/port/link.rsp), or None if there is no build"""
    rsp = os.path.join(build.OUT, "link.rsp")
    if not os.path.exists(rsp):
        return None
    out = set()
    with open(rsp) as f:
        for line in f:
            line = line.strip().replace("\\", "/")
            i = line.find("/build/port/obj/")
            if i >= 0 and line.endswith(".o"):
                out.add(line[i + len("/build/port/obj/"):-2] + ".c")
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count())
    ap.add_argument("-O", dest="opt", default="1")
    ap.add_argument("-o", "--output", default=REPORT)
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("files", nargs="*", help="only these sources (for testing; definitions elsewhere are missed)")
    args = ap.parse_args()
    t0 = time.time()

    all_jobs = jobs(args.opt)
    if args.files:
        want = {os.path.relpath(os.path.abspath(f), ROOT).replace("\\", "/") for f in args.files}
        all_jobs = [j for j in all_jobs if j[0] in want]
    linked = linked_libs()
    # a library source the build doesn't link: the C runtime replaces it, so
    # its calls don't run and its definitions are only a stand-in for the CRT's
    kind_of = {j[0]: ("lib-unlinked" if j[1] == "lib" and linked is not None and j[0] not in linked else j[1])
               for j in all_jobs}
    binary_of = {j[0]: j[2] for j in all_jobs}
    clang = build.find_clang()
    tmp = tempfile.mkdtemp(prefix="proto_audit_")
    results = []
    failed = []
    try:
        with multiprocessing.Pool(args.jobs, initializer=_init, initargs=(tmp, clang, sdl_include())) as pool:
            for n, (rel, err, decls, calls) in enumerate(pool.imap_unordered(dump_one, all_jobs, chunksize=4)):
                if err:
                    failed.append((rel, err))
                results.append((rel, decls, calls))
                if args.verbose and n % 250 == 0:
                    print(f"{n}/{len(all_jobs)} {time.time() - t0:.0f}s", file=sys.stderr)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    t_dump = time.time() - t0

    # Definitions: external by name, internal by (name, tu)
    defs = collections.defaultdict(dict)     # name -> {(file, line): def}
    idefs = {}
    decl_info = {}
    for rel, decls, calls in results:
        for d in decls:
            d["tu"] = rel
            d["src_kind"] = kind_of[rel]
            if d["def"] and d["sig"] is not None:
                if d["internal"]:
                    idefs[(d["name"], rel)] = d
                else:
                    defs[d["name"]].setdefault((d["file"], d["line"]), d)
            elif not d["def"]:
                decl_info.setdefault(d["key"], d)

    def def_for(name, rel, internal=False):
        if internal:
            return idefs.get((name, rel))
        ds = defs.get(name)
        if not ds:
            return None
        ds = list(ds.values())
        if len(ds) == 1:
            return ds[0]
        # one in the same binary, else a game one, else the first
        b = binary_of.get(rel)
        tier = {"crt": 1, "lib-unlinked": 2}
        rank = lambda d: (tier.get(d["src_kind"], 0), binary_of.get(d["tu"]) != b, d["file"])  # noqa: E731
        return sorted(ds, key=rank)[0]

    # Group the calls by what they call through: the declaration they see,
    # their cast, or (implicit) nothing
    groups = collections.OrderedDict()

    def group(key, **kw):
        g = groups.get(key)
        if g is None:
            g = groups[key] = dict(kw, calls=[])
        return g

    funcs = {}
    for rel, decls, calls in results:
        for d in decls:
            if d["def"]:
                funcs[(rel, d["name"])] = d["params"]
    n_calls = 0
    skipped = 0
    for rel, decls, calls in results:
        if kind_of[rel] == "lib-unlinked":
            skipped += len(calls)
            continue
        for c in calls:
            if c["file"].startswith("<sys>"):
                continue        # inside the C runtime's own inline functions
            n_calls += 1
            if c["cast"]:
                # the classes of the arguments are those of the cast's
                # parameters (each argument is converted to one)
                key = ("cast", c["file"], c["line"], c["col"], c["name"], rel)
                g = group(key, how="cast", name=c["name"], file=c["file"], line=c["line"], tu=rel,
                          type=c["ctype"], sig=None, internal=c["internal"])
            elif c["decl"] is None:
                key = ("implicit", rel, c["name"])
                g = group(key, how="implicit", name=c["name"], file=rel, line=c["line"], tu=rel,
                          type="int ()", sig=("i", (), False, True), internal=False)
            else:
                d = decl_info.get(c["decl"])
                if d is None:       # the definition itself, or a declaration in its TU
                    key = ("def", c["decl"], rel)
                    g = group(key, how="def", name=c["name"], file=c["decl"][1], line=c["decl"][2], tu=rel,
                              type=None, sig=c["decl_sig"], internal=c["internal"])
                else:
                    key = ("decl",) + tuple(c["decl"])
                    g = group(key, how="decl", name=c["name"], file=d["file"], line=d["line"], tu=d["tu"],
                              type=d["type"], sig=d["sig"], internal=d["internal"])
            g["calls"].append(c)
    # declarations nothing calls through still get the static check
    for key, d in decl_info.items():
        if kind_of.get(d["file"]) == "lib-unlinked" or d["src_kind"] == "crt" or d["file"].startswith("<sys>"):
            continue
        group(("decl",) + tuple(key), how="decl", name=d["name"], file=d["file"], line=d["line"], tu=d["tu"],
              type=d["type"], sig=d["sig"], internal=d["internal"])

    # Each group against its definition
    rows = []           # (harmful, kinds, group, def, call checks)
    nodef = collections.defaultdict(list)
    for key, g in groups.items():
        if g["name"].startswith("__builtin"):
            continue
        cl = g["calls"]
        if g["how"] == "cast":
            g["sig"] = _cast_sig(cl[0])
        tu = cl[0]["tu"] if cl else g["tu"]
        d = def_for(g["name"], tu, g["internal"])
        if d is None:
            if not g["internal"]:
                nodef[g["name"]].append(g)
            continue
        if g["how"] == "def" and d["file"] == g["file"] and d["line"] == g["line"]:
            kinds = []
        else:
            kinds = compare(g["sig"], d["sig"]) if g["sig"] else ["unparsed"]
        checks = []
        for c in cl:
            dd = def_for(g["name"], c["tu"], g["internal"]) or d
            w = harm(c, dd["sig"])
            if w:
                checks.append((c, dd, w))
        if kinds or checks:
            rows.append((bool(checks), kinds, g, d, checks))

    # No C definition: the declarations against each other
    nd_rows = []
    for name, gs in nodef.items():
        sigs = collections.Counter()
        for g in gs:
            if g["sig"] is not None and not g["sig"][3]:
                sigs[g["sig"]] += max(1, len(g["calls"]))
        if len(sigs) < 2:
            continue
        ref = sigs.most_common(1)[0][0]
        for g in gs:
            if g["sig"] is None or g["sig"] == ref:
                continue
            kinds = compare(g["sig"], ref)
            if g["sig"][3]:
                kinds = [k for k in kinds if k == "no-prototype"] + [k for k in kinds if k.startswith("return-class")]
            if kinds:
                nd_rows.append((name, g, ref, kinds, sigs))

    write_report(args.output, rows, nd_rows, funcs, failed, len(all_jobs), n_calls, skipped, t_dump, time.time() - t0)


def _cast_sig(c):
    """A cast call's signature: what it pushes (the argument classes, exact)
    and the result it reads"""
    s = parse_fn(c["ctype"], {}) or ("?", (), False, True)
    return (c["ret"], tuple(c["args"]) if not s[2] else tuple(c["args"][:len(s[1])]), s[2], s[3])


def write_report(path, rows, nd_rows, funcs, failed, n_files, n_calls, skipped, t_dump, t_all):
    def loc(c):
        return f"{c['file']}:{c['line']}"

    def where(g):
        return f"{g['file']}:{g['line']}"

    def dloc(d):
        tag = {"crt": " (C runtime)", "lib-unlinked": " (library source; the C runtime's is what links)",
               "lib": " (library source, linked)", "platform": " (platform layer)"}.get(d.get("src_kind"), "")
        return f"{d['file']}{tag}"

    def gdesc(g):
        how = {"cast": "cast at call", "implicit": "implicit declaration", "decl": "declared", "def": "defining file"}[g["how"]]
        t = g["type"] or ""
        return f"{how} {where(g)}: {t} = {sig_str(g['sig'])}"

    harmful = [r for r in rows if r[0]]
    static_only = [r for r in rows if not r[0] and r[1]]
    by_kind = collections.Counter()
    by_kind_harm = collections.Counter()
    for h, kinds, g, d, checks in rows:
        k = kinds[0] if kinds else "call-args"
        by_kind[k] += 1
        if h:
            by_kind_harm[k] += 1
    n_harm_calls = sum(len(r[4]) for r in harmful)
    nd_kind = collections.Counter(r[3][0] for r in nd_rows)

    def sort_key(r):
        h, kinds, g, d, checks = r
        return (g["name"], g["file"], g["line"])

    L = []
    L.append("Prototype audit: declarations and casts that pass or return differently on i686 than the definition")
    L.append(f"{n_files} sources, {n_calls} resolved calls (and {skipped} in library sources the C runtime replaces, "
             f"not checked), {t_all:.0f}s (dumps {t_dump:.0f}s). port/tools/proto_audit.py")
    L.append("")
    L.append("Classes: int = int, pointer, enum, char/short; f32 = float; f64 = double; s64 = 64-bit integer.")
    L.append("A call is HARMFUL when the classes it pushes are not, in order, those the definition reads, or it")
    L.append("pushes too few, or one side returns in st(0) and the other doesn't, or it reads a result never set.")
    L.append("")
    L.append("Counts by kind (first kind of each declaration/cast; 'call-args' = only the call's own arguments differ):")
    L.append(f"  {'kind':22} {'all':>5} {'harmful':>8}")
    for k in KIND_ORDER + ["call-args", "unparsed"]:
        if by_kind[k]:
            L.append(f"  {k:22} {by_kind[k]:5} {by_kind_harm[k]:8}")
    L.append(f"  {'total':22} {sum(by_kind.values()):5} {len(harmful):8}   ({n_harm_calls} harmful call sites)")
    L.append("")
    L.append(f"No C definition anywhere, declarations disagree with the most common one: {len(nd_rows)} "
             f"({', '.join(f'{k} {v}' for k, v in nd_kind.most_common())})")
    if failed:
        L.append(f"Sources clang rejected (their dumps may be partial): {len(failed)}: "
                 + " ".join(f for f, _ in sorted(failed)))
    L.append("")
    L.append("=" * 100)
    L.append("HARMFUL: one line per declaration or cast, then each call through it and what it should be in the port")
    L.append("  callee | definition file: signature | how declared, file:line: type = classes | kinds")
    L.append("=" * 100)
    for h, kinds, g, d, checks in sorted(harmful, key=sort_key):
        L.append(f"{g['name']} | def {dloc(d)}: {d['type']} = {sig_str(d['sig'])} | {gdesc(g)} | "
                 f"{','.join(kinds) or 'call-args'} | calls: {' '.join(loc(c) for c, _, _ in checks)}")
        for c, dd, why in checks:
            fx = fix_call(c, dd, funcs.get((c["tu"], c["func"]), []))
            L.append(f"    {loc(c)} in {c['func']}: {'; '.join(why)}")
            rdef, rcall = norm(dd["sig"][0], True), norm(c["ret"], True)
            if rdef != rcall and any("result" in w or "returns" in w for w in why):
                if rdef == "v":
                    fx = (fx or c["name"] + "(...)") + (f"  [returns nothing: on the EE the caller read whatever "
                                                         f"{c['name']} left in $v0]")
                else:
                    fx = (fx or c["name"] + "(...)") + f"  [returns {cname(dd['sig'][0])}: read it as that]"
            if fx:
                L.append(f"        -> {fx}")
    L.append("")
    L.append("=" * 100)
    L.append("HARMLESS: declarations or casts that differ, but no call through them passes or reads anything wrong")
    L.append("(unused declarations, extra trailing arguments, unused or void results, f32/f64 returns)")
    L.append("=" * 100)
    for h, kinds, g, d, checks in sorted(static_only, key=sort_key):
        calls = g["calls"]
        L.append(f"{g['name']} | def {dloc(d)}: {d['type']} = {sig_str(d['sig'])} | {gdesc(g)} | {','.join(kinds)} | "
                 + (f"calls (all harmless): {' '.join(loc(c) for c in calls)}" if calls else "no calls"))
    L.append("")
    L.append("=" * 100)
    L.append("NO C DEFINITION (assembly or the C runtime): declarations that disagree with the most common one")
    L.append("  callee | reference (most common, call-weighted) | how declared: type = classes | kinds | calls")
    L.append("=" * 100)
    for name, g, ref, kinds, sigs in sorted(nd_rows, key=lambda r: (r[0], r[1]["file"], r[1]["line"])):
        others = "; ".join(f"{sig_str(s)} x{n}" for s, n in sigs.most_common())
        L.append(f"{name} | ref {sig_str(ref)} (all: {others}) | {gdesc(g)} | {','.join(kinds)} | "
                 + (f"calls: {' '.join(loc(c) for c in g['calls'])}" if g["calls"] else "no calls"))
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(L) + "\n")
    # summary
    print("\n".join(L[:L.index("=" * 100)]))
    print(f"report: {os.path.relpath(path, ROOT)}")


if __name__ == "__main__":
    main()
