#!/usr/bin/env python3
"""Give symbols real names everywhere at once.

Usage: python3 tools/rename.py NAMES.txt... [--dry-run] [--allow-used]

Each line of a names file is `OLD NEW [// comment]`: OLD is an address-derived
name (func_0040A1B0, D_004A9480) or a name already in the symbol files, NEW
the name to give it. Blank lines and lines starting with # are skipped.

For each pair this
  - records NEW at OLD's address in the binary's symbol file
    (config/symbol_addrs.txt for the main executable and anything shared,
    config/symbol_addrs.<overlay>.txt for an overlay's own symbols), so
    splat labels the assembly with it, keeping the comment;
  - replaces OLD with NEW as a whole word in src/ and include/.

It refuses names that aren't C identifiers, that are already in use
(outside comments and string literals), or that are given twice.
--allow-used accepts names already used in the source, for a library name
some file already writes (`#define strlen func_00482E9C`, a local
prototype of memcpy, a struct field): such aliases are removed, and the
rebuild shows whether anything else clashed. Run tools/split.sh and make afterwards (see
docs/naming.md): a few functions' register allocation depends on the hash
of the global names they use, so a rename can break a match.
"""
import glob
import re
import sys

IDENT = re.compile(r"^[A-Za-z_]\w*$")
ADDR_NAME = re.compile(r"^(?:func|D)_([0-9A-F]{8})$")
SYM_LINE = re.compile(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)$")
# string and character literals, and comments: names there aren't "in use",
# and replacements never touch the literals
LITERAL = re.compile(r'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'|/\*.*?\*/|//[^\n]*', re.S)
C_KEYWORDS = set("""auto break case char const continue default do double else enum extern
float for goto if inline int long register restrict return short signed sizeof static struct
switch typedef union unsigned void volatile while asm __asm__""".split())
RANGES = [  # (binary, start, end): each overlay's own image; anything else
    # (the main executable, runtime buffers such as BATTLE's GBX load area
    # at 0x00708000) goes in the shared symbol file
    ("field", 0x005B0000, 0x0067F640),
    ("battle", 0x00680000, 0x006F9350),
    ("viewer", 0x00700000, 0x0070DF44),
]


def sym_file(binary):
    return "config/symbol_addrs.txt" if binary == "main" else f"config/symbol_addrs.{binary}.txt"


def binary_of(addr):
    for b, lo, hi in RANGES:
        if lo <= addr < hi:
            return b
    return "main"


def load_symbols():
    syms = {}  # name -> (addr, file)
    for path in sorted(glob.glob("config/symbol_addrs*.txt")):
        for line in open(path):
            m = SYM_LINE.match(line.strip())
            if m:
                syms[m.group(1)] = (int(m.group(2), 16), path, "type:func" in m.group(3))
    return syms


def parse(paths):
    pairs = []
    for path in paths:
        for n, line in enumerate(open(path), 1):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            body, _, comment = line.partition("//")
            parts = body.split()
            if len(parts) != 2:
                sys.exit(f"{path}:{n}: expected `OLD NEW [// comment]`")
            pairs.append((parts[0], parts[1], comment.strip(), f"{path}:{n}"))
    return pairs


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    dry = "--dry-run" in sys.argv
    allow = "--allow-used" in sys.argv
    if not args:
        sys.exit(__doc__)
    pairs = parse(args)
    syms = load_symbols()
    files = sorted(glob.glob("src/**/*.c", recursive=True) + glob.glob("include/**/*.h", recursive=True))
    files = [f for f in files if "pending_inline_asm" not in f]
    texts = {f: open(f).read() for f in files}
    used = set()
    for t in texts.values():
        used.update(re.findall(r"\b[A-Za-z_]\w*\b", LITERAL.sub(" ", t)))
    used.update(syms)

    errors = []
    olds, news = set(), set()
    plan = []
    for old, new, comment, where in pairs:
        if not IDENT.match(new) or new in C_KEYWORDS or ADDR_NAME.match(new):
            errors.append(f"{where}: bad name {new}")
        elif new in news:
            errors.append(f"{where}: {new} is given twice")
        elif new in used:
            if allow:
                print(f"{where}: note: {new} is already used in the source (allowed)")
            else:
                errors.append(f"{where}: {new} is already in use")
        if old in olds:
            errors.append(f"{where}: {old} renamed twice")
        m = ADDR_NAME.match(old)
        if m:
            addr = int(m.group(1), 16)
        elif old in syms:
            addr = syms[old][0]
        else:
            errors.append(f"{where}: unknown symbol {old}")
            continue
        olds.add(old)
        news.add(new)
        kind = "func" if old.startswith("func_") or (old in syms and syms[old][2]) else "data"
        plan.append((old, new, addr, kind, comment))
    if errors:
        sys.exit("\n".join(errors))

    # symbol files
    by_file = {}
    for old, new, addr, kind, comment in plan:
        path = syms[old][1] if old in syms else sym_file(binary_of(addr))
        by_file.setdefault(path, []).append((old, new, addr, kind, comment))
    for path, items in by_file.items():
        lines = open(path).read().splitlines()
        renamed = {old: new for old, new, *_ in items}
        out = []
        for line in lines:
            m = SYM_LINE.match(line.strip())
            if m and m.group(1) in renamed:
                line = renamed.pop(m.group(1)) + line.strip()[len(m.group(1)):]
            out.append(line)
        added = [it for it in items if it[0] in renamed]
        if added:
            out.append("")
            for old, new, addr, kind, comment in sorted(added, key=lambda it: it[2]):
                comment = comment.replace(":", " -").replace(";", ",")  # splat reads key:value words as attributes and wants one ;
                # splat has no data type; data symbols take their size from use
                typ = "type:func" + ("  " if comment else "") if kind == "func" else ""
                out.append(f"{new} = 0x{addr:08X};" + (f" // {typ}{comment}" if typ or comment else ""))
        if not dry:
            open(path, "w").write("\n".join(out) + "\n")

    # sources
    mapping = {old: new for old, new, *_ in plan}
    pattern = re.compile(r"\b(" + "|".join(map(re.escape, sorted(mapping, key=len, reverse=True))) + r")\b")
    changed = 0
    for f, t in texts.items():
        # rename in code and comments, never inside string literals
        parts, pos = [], 0
        for m in LITERAL.finditer(t):
            parts.append(pattern.sub(lambda x: mapping[x.group(1)], t[pos:m.start()]))
            lit = m.group(0)
            parts.append(lit if lit[0] in "\"'" else pattern.sub(lambda x: mapping[x.group(1)], lit))
            pos = m.end()
        parts.append(pattern.sub(lambda x: mapping[x.group(1)], t[pos:]))
        nt = "".join(parts)
        # `#define strlen func_00482E9C` style aliases become `#define strlen strlen`
        nt = re.sub(r"^#define\s+(\w+)\s+\1\s*\n", "", nt, flags=re.M)
        if nt != t:
            changed += 1
            if not dry:
                open(f, "w").write(nt)
    print(f"{len(plan)} names, {changed} files changed" + (" (dry run)" if dry else ""))


if __name__ == "__main__":
    main()
