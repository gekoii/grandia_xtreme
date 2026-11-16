#!/usr/bin/env python3
"""Add the coprocessor hazard nop the game's assembler put at `.set noreorder`.

Used by the `/* as: ee-gcc2.96-hazard */` marker (see tools/compile_c.py).

The game's code was assembled with an EE gas that had both of these:
  - the 2.96 toolchain's delay-slot filling (e.g. `mtc1; b L; cvt.s.w` with
    the cvt moved into the branch's delay slot), which the 1999 assemblers
    never do, and
  - the 1999 assembler's hazard nop when a `.set noreorder` block starts
    right after a coprocessor hazard, which the 2.96 assembler skips because
    it treats the R5900 as interlocked.
None of the assemblers in tools/compilers does both, so this pass adds the
nop to the compiler's output and the stock 2.96 assembler does the rest.

The rule (from the 1999 assembler's behaviour, and checked against the
game): when gcc opens a `.set noreorder` block (a branch whose delay slot it
filled itself), and the last instruction before it is
  - an FP compare (c.cond.s/d) and the block's first instruction is a
    bc1t/bc1f/bc1tl/bc1fl, or
  - mtc1/dmtc1/ctc1 or mfc1/dmfc1/cfc1 and the block's first instruction
    uses the register it moves,
a nop goes right after that instruction (before any labels in between).
Reorder-mode code is untouched; the assembler's own scheduling handles it.

Usage: as_hazard.py <in.s> <out.s>
"""
import re
import sys

REG = re.compile(r"\$(?:f?\d+|[a-z]+\d*)")
NOP = "\t.set\tnoreorder\n\tnop\t# coprocessor hazard (tools/as_hazard.py)\n\t.set\treorder"


def operands(args):
    return [x.strip() for x in args.split("#")[0].split(",")]


def instruction(line):
    """(mnemonic, operands) of an instruction line, or None for anything else."""
    s = line.split("#")[0].strip()
    if not s or s.endswith(":") or s.startswith("."):
        return None
    m = re.match(r"(\S+)\s*(.*)", s)
    return m.group(1), m.group(2)


def hazard(prev, first):
    op, args = prev
    nxt, nargs = first
    if re.fullmatch(r"c\.[a-z]+\.[sd]", op) and re.fullmatch(r"bc1[tf]l?", nxt):
        return True
    ops = operands(args)
    used = set(REG.findall(nargs.split("#")[0]))
    if op in ("mtc1", "dmtc1", "ctc1") and len(ops) == 2 and ops[1] in used:
        return True
    if op in ("mfc1", "dmfc1", "cfc1") and len(ops) == 2 and ops[0] in used:
        return True
    return False


def add_hazard_nops(text):
    out = []
    noreorder = False
    prev = prev_at = None
    pending = False
    for line in text.split("\n"):
        st = line.strip()
        if st.startswith(".set") and "noreorder" in st:
            noreorder = True
            pending = prev is not None
            out.append(line)
            continue
        if st.startswith(".set") and re.search(r"\breorder\b", st):
            noreorder = False
            prev = None
            out.append(line)
            continue
        ins = instruction(line)
        if ins and noreorder and pending:
            pending = False
            if hazard(prev, ins):
                out.insert(prev_at + 1, NOP)
        if ins and not noreorder:
            prev, prev_at = ins, len(out)
        if st.startswith((".end", ".ent")):
            prev = None
        out.append(line)
    return "\n".join(out)


def main():
    src, dst = sys.argv[1:3]
    with open(src) as f:
        text = f.read()
    with open(dst, "w") as f:
        f.write(add_hazard_nops(text))


if __name__ == "__main__":
    main()
