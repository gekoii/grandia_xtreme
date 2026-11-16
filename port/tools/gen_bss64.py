#!/usr/bin/env python3
"""What changes size or place on x86-64 among the objects the port lays
out at their PS2 offsets (port/docs/win64.md): writes
port/tools/fixups/bss64.json for fixups/objects.py and syms.py.

    python port/tools/gen_bss64.py            # writes the file
    python port/tools/gen_bss64.py --check    # only says whether it is current

The port makes each .bss file one block, every object an alias at its PS2
offset (fixups/objects.py), with the slot gen_bss.py gave it: u32 for a
pointer. The matching build's linker also defines names as another
object's address plus an offset (a field, or a variable without a label
of its own), which syms.py turns into `.set`s. On x86-64 a pointer is 8
bytes and many structs hold pointers, so objects of both kinds can run
into their neighbours there, and a field's offset can change.

Neither the bss files nor the linker scripts say the objects' types; the
game's declarations do. So each name's sizeof is measured with clang, for
i686 and for x86-64, in the sources that declare it (a line appended to
build.py's generated copy; a declaration inside a function is repeated at
file scope for it). Declarations of one name that disagree take the
largest. A name grows when its x86-64 size is above its i686 one.

A linker-defined name inside its base (base + offset, the offset within
the base's i686 size) is a field. If the base grows, the field may move:
it is found in the base's i686 layout (clang's record layout dump, member
by member, arrays by element), and its x86-64 offset measured as the
address difference of that C expression, in a source that defines the
base (else one that declares it). The i686 offset must come out as the
linker's.

Output:

  "bss":     {"<bss file>": {"<name>": <x86-64 size>}}: the growing bss
             objects; objects.py defines each on its own there, outside
             the block, and its slot stays unused.
  "aliases": {"<name>": ["own", <size>]}: a linker-defined name past its
             base's end (a variable of its own, merged into the slot
             before it) that grows: its own object there;
             {"<name>": ["set", "<base>", <offset>]}: an alias to put
             elsewhere on x86-64: onto the block, for a name past the end
             of a base that moved out, or at a field's x86-64 offset.

Run it after a change to a type in include/.
"""

import argparse
import collections
import concurrent.futures
import itertools
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
import build  # noqa: E402
import syms  # noqa: E402
from fixups import objects  # noqa: E402

OUT_JSON = os.path.join(build.ROOT, "port", "tools", "fixups", "bss64.json")
I686, X64 = TRIPLES = ("i686-w64-mingw32", "x86_64-w64-mingw32")
PROBE = "unsigned long long grx_sz_{0} = {1};"
SIZE_RE = re.compile(r"^@grx_sz_(\w+) = (?:dso_local )?global i64 (\d+)", re.M)
ERR_RE = re.compile(r"grx_probe:(\d+):\d+: error:")
PROBE_LINE = 1000000
TMP = os.path.join(build.ROOT, "build", "skip", "bss64")


def bss_objects():
    """{name: (bss file, slot size, block name, offset in the block)}"""
    out = {}
    for rel in objects.BSS_FILES:
        with open(os.path.join(build.ROOT, rel), encoding="latin-1") as f:
            text = f.read()
        lo = int(objects._BSS_RANGE.search(text[:text.index("*/")]).group(1), 16)
        block = f"grx_{os.path.dirname(rel).split('/')[-1]}_{os.path.basename(rel)[:-2]}"
        for d in re.finditer(objects._BSS_DEF, text):
            typ, name, count, addr = d.group(1), d.group(2), d.group(3), int(d.group(5), 16)
            out[name] = (rel, objects._SIZES[typ] * (int(count, 16) if count else 1), block, addr - lo)
    return out


# An extern declaration inside a function (indented): the probe, at file
# scope, repeats it there
LOCAL_EXTERN = re.compile(r"^[ \t]+extern\s+([^;(){}=]*?\b(\w+)\s*(?:\[[^\]]*\])*)\s*;", re.M)


# The x86-64 build's copies (port/tools/ee32.py's rewrite, where the game's
# structs keep the PS2's layout): measured there, not in build.py's
X64_OUT = os.path.join(build.ROOT, "build", "port64")


def _probe_text(rel, body_for, triple=None):
    if triple == X64:
        src = os.path.join(X64_OUT, "gen64", rel)
        if not os.path.exists(src):
            raise SystemExit(f"{src} isn't there: build the x86-64 port first (build.py --target x86_64), "
                             "which writes it before it checks bss64.json")
    else:
        src = build.generate(rel, out_dir=os.path.join(TMP, "gen"))
    with open(src, encoding="latin-1") as f:
        base = f.read()
    # Not what syms.py appended from the last bss64.json (an own object it
    # placed would measure as that again): the game's declarations only
    cut = base.find("/* Names the matching build's linker defines (port/tools/syms.py) */")
    if cut >= 0:
        base = base[:cut]
    local = {m.group(2): m.group(1) for m in LOCAL_EXTERN.finditer(base)}

    def decls(expr):
        return "".join(f"extern {local[n]}; " for n in dict.fromkeys(re.findall(r"\b\w+\b", expr)) if n in local)
    return base + f'\n#line {PROBE_LINE} "grx_probe"\n' + body_for(decls)


_seq = itertools.count()


def _run(clang, rel, text, triple, extra=()):
    # A file of its own per run: probes of one source run at once
    path = os.path.join(TMP, "probe", triple, f"{rel[:-2]}.{next(_seq)}.c")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    flags = [f for f in build.game_flags(rel, "0") if f != "-w"] + ["-w"]
    if triple == X64:
        flags = [os.path.join(X64_OUT, "hdr64") if f == os.path.join(build.OUT, "hdr") else f for f in flags]
    return subprocess.run([clang, "-target", triple, "-S", "-emit-llvm", "-o", "-", path] + flags + list(extra),
                          capture_output=True, text=True, cwd=build.ROOT)


def probe(clang, rel, exprs, triple):
    """{key: value} of the {key: C expression} clang can evaluate in rel's
    generated copy"""
    keys = sorted(exprs)
    while keys:
        # The probes number their own lines (the copy has #line directives
        # of its own): probe i is line PROBE_LINE + i
        text = _probe_text(rel, lambda decls: "\n".join(decls(exprs[k]) + PROBE.format(k, exprs[k])
                                                         for k in keys) + "\n", triple)
        p = _run(clang, rel, text, triple)
        if p.returncode == 0:
            return {m.group(1): int(m.group(2)) for m in SIZE_RE.finditer(p.stdout)}
        # Drop the probes that failed (undeclared here, incomplete type), by
        # their line
        bad = {int(m.group(1)) - PROBE_LINE for m in ERR_RE.finditer(p.stderr)}
        bad = {i for i in bad if 0 <= i < len(keys)}
        if not bad:
            return {}
        keys = [k for i, k in enumerate(keys) if i not in bad]
    return {}


LAYOUT_LINE = re.compile(r"^\s*(\d+)(:\d+-\d+)? \|( +)(.*)$")


def layout(clang, rel, expr):
    """(type text, [(offset, member name, type text)], sizeof) of expr's
    type on i686, from clang's record layout dump: its direct members"""
    text = _probe_text(rel, lambda decls: decls(expr) + f"struct grx_layout_t {{ __typeof__({expr}) v; }}; "
                       + PROBE.format("layout", "sizeof(struct grx_layout_t)") + "\n")
    p = _run(clang, rel, text, I686, ["-Xclang", "-fdump-record-layouts"])
    at = p.stdout.find("| struct grx_layout_t\n")
    if p.returncode or at < 0:
        raise RuntimeError(f"{rel}: can't lay out {expr}: {p.stderr[:600]}")
    dump = p.stdout[at:]
    dump = dump[:dump.index("[sizeof=")]
    vtype, members = None, []
    for line in dump.split("\n")[1:]:
        m = LAYOUT_LINE.match(line)
        if not m or m.group(2):
            continue  # bit-fields: no byte of their own
        depth = (len(m.group(3)) - 1) // 2
        decl = m.group(4).rsplit(" ", 1)
        if depth == 1:
            vtype = decl[0]
        elif depth == 2:
            members.append((int(m.group(1)), decl[1] if len(decl) == 2 else "", decl[0]))
    size = int(SIZE_RE.search(p.stdout).group(2))
    return vtype, members, size


def field_expr(clang, rel, base, off):
    """The C expression for the member at i686 offset off in base"""
    expr, r = base, off
    for _ in range(16):
        if r == 0:
            return expr
        vtype, members, size = layout(clang, rel, expr)
        if not members:
            # No members of its own: an array (the dump names the type
            # typeof (...), so it can't say), by its element's size
            esize = probe(clang, rel, {"e": f"sizeof(({expr})[0])"}, I686).get("e")
            if not esize or esize >= size:
                raise RuntimeError(f"{base} + 0x{off:X}: {expr} ({vtype}) is neither a struct nor an array")
            idx = r // esize
            expr, r = f"({expr})[{idx}]", r - idx * esize
            continue
        inside = [m for m in members if m[0] <= r]
        if not inside or not inside[-1][1]:
            raise RuntimeError(f"{base} + 0x{off:X}: no named member at {r} in {expr} ({vtype})")
        o, name, _ = inside[-1]
        expr, r = f"({expr}).{name}", r - o
    raise RuntimeError(f"{base} + 0x{off:X}: too deep")


DEFINES = r"^(?!extern\b)[A-Za-z_][\w \t*]*\b{0}\s*(?:\[[^\]]*\])*\s*[=;]"


def measure(jobs, names):
    """({triple: {name: largest sizeof}}, {name: [the sources it was measured in, definers first]})"""
    word = re.compile(r"\b(" + "|".join(sorted(names, key=len, reverse=True)) + r")\b")
    clang = build.direct_clang(build.find_clang())
    work, texts = [], {}
    for rel, _, host in build.game_sources():
        if host or rel in objects.BSS_FILES:
            continue
        with open(os.path.join(build.ROOT, rel), encoding="latin-1") as f:
            texts[rel] = f.read()
        found = set(word.findall(texts[rel]))
        if found:
            work.extend((rel, {n: f"sizeof({n})" for n in found}, t) for t in TRIPLES)
    sizes = {t: collections.defaultdict(set) for t in TRIPLES}
    where = collections.defaultdict(list)
    with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
        for (rel, _, t), res in zip(work, ex.map(lambda w: probe(clang, *w), work)):
            for n, v in res.items():
                sizes[t][n].add(v)
                if t == I686:
                    where[n].append(rel)
    for n, rels in where.items():
        rels.sort(key=lambda r: not re.search(DEFINES.format(re.escape(n)), texts[r], re.M))
    return {t: {n: max(v) for n, v in sizes[t].items()} for t in TRIPLES}, where


def classify(jobs, objs, rel_names, s32, s64, where):
    clang = build.direct_clang(build.find_clang())
    grows = {n for n in s64 if n in s32 and s64[n] > s32[n]}
    out = {"bss": collections.defaultdict(dict), "aliases": {}}
    for name in sorted(grows & set(objs)):
        out["bss"][objs[name][0]][name] = s64[name]
    problems, fields = [], []
    for name, (base, off, _) in sorted(rel_names.items()):
        if name in objs or off == 0:
            continue  # off 0: another name for its base, which stays one
        base32 = s32.get(base, objs[base][1] if base in objs else None)
        if base32 is None:
            if name in grows:
                problems.append(f"{name} = {base} + 0x{off:X} grows, and {base}'s size is unknown")
            continue
        if off >= base32:
            # A variable of its own after its base
            if name in grows:
                out["aliases"][name] = ["own", s64[name]]
            elif base in objs and base in grows:
                _, _, block, boff = objs[base]
                out["aliases"][name] = ["set", block, boff + off]
        elif base in grows:
            fields.append((name, base, off))
        elif name in grows:
            # Inside a base without pointers (sndIopHeader in the 0xC bytes
            # of s32 sndIopStatusAddr[3]): no field of it can be a pointer,
            # so it is a variable of its own the split merged
            out["aliases"][name] = ["own", s64[name]]

    def resolve(f):
        """The field in the first source (definers first) whose layout of
        the base has it at the linker's offset: the decompilation declares
        some objects differently in different files"""
        name, base, off = f
        tried = []
        for rel in where[base]:
            try:
                expr = field_expr(clang, rel, base, off)
            except RuntimeError as e:
                tried.append(str(e))
                continue
            d = f"(unsigned long long)((char *)&({expr}) - (char *)&({base}))"
            o32 = probe(clang, rel, {"o": d}, I686).get("o")
            o64 = probe(clang, rel, {"o": d}, X64).get("o")
            if o32 == off and o64 is not None:
                return name, (base, o64, expr), None
            tried.append(f"{expr} in {rel} is at {o32} / {o64}")
        return name, None, f"{name} = {base} + 0x{off:X}: " + "; ".join(tried)

    with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
        for name, res, err in ex.map(resolve, fields):
            if err:
                problems.append(err)
            elif res[1] != rel_names[name][1]:
                out["aliases"][name] = ["set", res[0], res[1]]
    out["bss"] = {rel: dict(sorted(v.items())) for rel, v in sorted(out["bss"].items())}
    out["aliases"] = dict(sorted(out["aliases"].items()))
    return out, problems


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count())
    ap.add_argument("--reuse", action="store_true", help="reuse the last run's sizes (only the fields again)")
    args = ap.parse_args()
    objs = bss_objects()
    rel_names = syms.LinkerNames(build.ROOT).rel
    names = set(objs) | set(rel_names) | {b for b, _, _ in rel_names.values() if re.fullmatch(r"\w+", b)}
    cache = os.path.join(TMP, "measured.json")
    if args.reuse and os.path.exists(cache):
        with open(cache) as f:
            sizes, where = json.load(f)
    else:
        sizes, where = measure(args.jobs, names)
        os.makedirs(TMP, exist_ok=True)
        with open(cache, "w") as f:
            json.dump([sizes, where], f)
    data, problems = classify(args.jobs, objs, rel_names, sizes[I686], sizes[X64], where)
    if problems:
        sys.exit("\n".join(problems))
    # What it was measured against: the x86-64 build checks it
    data = {"headers": build.headers_signature(), **data}
    text = json.dumps(data, indent=1) + "\n"
    old = open(OUT_JSON).read() if os.path.exists(OUT_JSON) else None
    if args.check:
        print("bss64.json is current" if old == text else "bss64.json is out of date: run port/tools/gen_bss64.py")
        sys.exit(0 if old == text else 1)
    with open(OUT_JSON, "w", newline="\n") as fo:
        fo.write(text)
    unsized = [n for n in objs if n not in sizes[X64]]
    print(f"{sum(len(v) for v in data['bss'].values())} bss objects grow on x86-64, "
          f"{len(data['aliases'])} linker-defined names move; {len(unsized)} bss objects never declared with a "
          f"complete type outside the bss files (kept in their slots)")


if __name__ == "__main__":
    main()
