#!/usr/bin/env python3
"""Pick global names that keep a function's gcse PRE order.

gcse numbers the pseudos it creates for partially redundant expressions in
hash-bucket order, and the bucket of `%hi(sym)` comes from the symbol's name:
(8320 + h(name)) mod N, with h += (h << 7) + c over the characters and N the
bucket count (docs/matching-tips.md). Renaming a global can therefore reorder
the pseudos and break a match. Given a file that no longer matches after a
rename and the version from before it (which matched), this reads both gcse
dumps, finds the `%hi` expressions whose order changed, and searches name
variants (the new name plus a descriptive suffix) that restore the old order.

Usage (in the container): hash_names.py NEW.c OLD.c
Prints suggested `old_name new_name` pairs, or why it couldn't.
"""

import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from compile_c import CFLAGS, toolchain  # noqa: E402

SUFFIXES = ["", "Buf", "Tbl", "Table", "Data", "Work", "Val", "Ptr", "List", "Info", "Main", "Base",
            "Var", "Arr", "Cur", "Value", "Area", "Str", "Fmt", "Text", "Msg", "Label", "Num", "Count",
            "Flags", "Param", "Params", "Cfg", "Def", "Desc", "Rec", "Entry", "Array", "Block", "State",
            "Mode", "Pos", "Size", "Obj", "Ref", "Addr", "Handle", "Id", "Index", "Slot", "Slots"]
HI = re.compile(r'\(high:SI \(symbol_ref:SI \("(\w+)"\)\)\)')


def h(name):
    x = 0
    for c in name.encode():
        x = (x + ((x << 7) & 0xFFFFFFFF) + c) & 0xFFFFFFFF
    return x


def bucket(name, n):
    return ((8320 + h(name)) & 0xFFFFFFFF) % n


def gcse_dump(path):
    cc, _, extra = toolchain(path)
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    flags = [f if not f.startswith("-I") else "-I" + os.path.join(root, f[2:]) for f in CFLAGS]
    with tempfile.TemporaryDirectory() as d:
        subprocess.run([os.path.join(root, f"tools/compilers/{cc}/bin/ee-gcc"), *flags, *extra, "-dG", "-S",
                        "-o", os.path.join(d, "x.s"), os.path.abspath(path)], cwd=d, capture_output=True)
        dumps = [f for f in os.listdir(d) if f.endswith(".gcse")]
        return open(os.path.join(d, dumps[0])).read() if dumps else ""


def functions(dump):
    """Per function: (bucket count, {expr index: (hash, text)}, ordered PRE'd expr indices)."""
    out = []
    for part in re.split(r"\n(?=;; Function )", dump):
        m = re.search(r"Expression hash table \((\d+) buckets", part)
        if not m:
            continue
        n = int(m.group(1))
        table = part[m.end():].split("\n\n", 1)[0]
        exprs = {}
        for em in re.finditer(r"Index (\d+) \(hash value (\d+)\)\n((?:  .*\n?)+)", table):
            exprs[int(em.group(1))] = (int(em.group(2)), " ".join(em.group(3).split()))
        order = []
        for rm in re.finditer(r"PRE: redundant insn \d+ \(expression (\d+)\) in bb \d+, reaching reg is (\d+)", part):
            e, r = int(rm.group(1)), int(rm.group(2))
            if e not in [o for o, _ in order]:
                order.append((e, r))
        order = [e for e, _ in sorted(order, key=lambda x: x[1])]
        out.append((n, exprs, order))
    return out


def main():
    new_path, old_path = sys.argv[1], sys.argv[2]
    new_f, old_f = functions(gcse_dump(new_path)), functions(gcse_dump(old_path))
    for (n, new_exprs, new_order), (n_old, old_exprs, old_order) in zip(new_f, old_f):
        old_syms = [HI.search(old_exprs[e][1]).group(1) if HI.search(old_exprs[e][1]) else None for e in old_order]
        new_syms = [HI.search(new_exprs[e][1]).group(1) if HI.search(new_exprs[e][1]) else None for e in new_order]
        if old_syms == new_syms or n != n_old:
            continue
        # the PRE'd expressions in the old order, with their new names
        renamed = {}
        for e in old_order:
            m = HI.search(old_exprs[e][1])
            if m:
                renamed[m.group(1)] = None
        # map old names to new: the new dump has the same %hi expressions in some order
        new_his = [HI.search(t).group(1) for _, t in new_exprs.values() if HI.search(t)]
        old_his = [HI.search(t).group(1) for _, t in old_exprs.values() if HI.search(t)]
        pairs = dict(zip(old_his, new_his))  # same expression indices in both tables
        target = [old_exprs[e][0] if not HI.search(old_exprs[e][1]) else None for e in old_order]
        names = [pairs.get(HI.search(old_exprs[e][1]).group(1)) if HI.search(old_exprs[e][1]) else None
                 for e in old_order]
        fixed = [old_exprs[e][0] for e in old_order]
        variable = [i for i, nm in enumerate(names) if nm and not nm.startswith(("D_", "func_"))]
        print(f"N={n}: old PRE order {[s or '-' for s in old_syms]}")
        # dynamic programming over the positions in the old order: the buckets
        # must rise as they did, changing as few names as possible
        def ok(prev_val, i, val):
            return prev_val < val or (prev_val == val and fixed[i - 1] == fixed[i])

        states = {None: (0, {})}  # bucket of the previous position -> (changes, suffixes)
        for i in range(len(fixed)):
            options = SUFFIXES if i in variable else [None]
            nxt = {}
            for prev, (cost, chosen) in states.items():
                for suf in options:
                    val = fixed[i] if suf is None else bucket(names[i] + suf, n)
                    if i and not ok(prev, i, val):
                        continue
                    c = cost + (1 if suf else 0)
                    if val not in nxt or c < nxt[val][0]:
                        nxt[val] = (c, {**chosen, i: suf} if suf else chosen)
            states = nxt
        best = None
        if states:
            cost, chosen = min(states.values(), key=lambda s: s[0])
            best = {names[j]: names[j] + s for j, s in chosen.items()}
        if best is None:
            print("  no suffix combination restores the order")
        else:
            for a, b in best.items():
                print(f"  {a} {b}")


if __name__ == "__main__":
    main()
