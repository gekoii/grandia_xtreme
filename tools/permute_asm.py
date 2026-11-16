#!/usr/bin/env python3
"""Hide inline-asm statements from the permuter, and put them back.

decomp-permuter parses candidates with pycparser, which can't read GNU
inline asm with operands (`__asm__ volatile("..." : "=r"(x) : "r"(p));`),
so files using the VU0 or lq/sq macros couldn't be permuted. `hide` turns
each asm statement into a call to a placeholder function whose arguments
are the operand expressions (outputs as `&(expr)`), so the permuter can
still move the statement and rename or rewrite its operands; `restore`
rebuilds the asm from the placeholder call before a candidate is compiled.

Usage (by tools/permute.py and its compile.sh):
    permute_asm.py hide IN.c OUT.c SPECS.json
    permute_asm.py restore FILE.c SPECS.json   (rewrites FILE.c in place)
"""
import json
import re
import sys

ASM_START = re.compile(r"\b(__asm__|asm|__asm)\s*(volatile|__volatile__)?\s*\(")
PLACEHOLDER = "__permuter_asm_"


def matching_paren(text, i):
    """Index of the parenthesis closing the one at text[i]."""
    depth = 0
    j = i
    while j < len(text):
        c = text[j]
        if c == '"':
            j += 1
            while text[j] != '"':
                j += 2 if text[j] == "\\" else 1
        elif c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return j
        j += 1
    raise ValueError("unbalanced parentheses")


def split_top(text, sep):
    """Split text at top-level occurrences of sep (outside parens and strings)."""
    parts, depth, cur, j = [], 0, "", 0
    while j < len(text):
        c = text[j]
        if c == '"':
            k = j + 1
            while text[k] != '"':
                k += 2 if text[k] == "\\" else 1
            cur += text[j:k + 1]
            j = k + 1
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += c
        j += 1
    parts.append(cur)
    return parts


def parse_operands(section):
    ops = []
    for op in split_top(section, ","):
        op = op.strip()
        if not op:
            continue
        m = re.match(r'^((?:"(?:[^"\\]|\\.)*"\s*)+)\((.*)\)$', op, re.S)
        if not m:
            raise ValueError(f"can't parse asm operand: {op}")
        ops.append((m.group(1).strip(), m.group(2).strip()))
    return ops


def hide(text):
    specs = []
    out = ""
    pos = 0
    while True:
        m = ASM_START.search(text, pos)
        if not m:
            break
        open_i = m.end() - 1
        close_i = matching_paren(text, open_i)
        sections = split_top(text[open_i + 1:close_i], ":")
        template = sections[0].strip()
        outs = parse_operands(sections[1]) if len(sections) > 1 else []
        ins = parse_operands(sections[2]) if len(sections) > 2 else []
        clobbers = sections[3].strip() if len(sections) > 3 else ""
        n = len(specs)
        specs.append({"keyword": m.group(1), "volatile": m.group(2) or "", "template": template,
                      "outs": [c for c, _ in outs], "ins": [c for c, _ in ins], "clobbers": clobbers,
                      "colons": len(sections) - 1})
        args = [f"&({e})" for _, e in outs] + [e for _, e in ins]
        out += text[pos:m.start()] + f"{PLACEHOLDER}{n}({', '.join(args)})"
        pos = close_i + 1
    out += text[pos:]
    if specs:
        decls = "".join(f"void {PLACEHOLDER}{i}();\n" for i in range(len(specs)))
        out = decls + out
    return out, specs


def restore(text, specs):
    text = re.sub(r"^void " + PLACEHOLDER + r"\d+\(\);\n", "", text, flags=re.M)
    for i, spec in enumerate(specs):
        call = re.compile(r"\b" + PLACEHOLDER + str(i) + r"\s*\(")
        while True:
            m = call.search(text)
            if not m:
                break
            close_i = matching_paren(text, m.end() - 1)
            inner = text[m.end():close_i]
            args = [a.strip() for a in split_top(inner, ",")] if inner.strip() else []
            n_out = len(spec["outs"])
            outs = []
            for c, a in zip(spec["outs"], args[:n_out]):
                if a.startswith("&"):
                    m2 = re.match(r"^&\s*\((.*)\)$", a, flags=re.S)
                    a = m2.group(1) if m2 else a[1:].strip()
                else:
                    a = f"*({a})"
                outs.append(f"{c}({a})")
            ins = [f"{c}({a})" for c, a in zip(spec["ins"], args[n_out:])]
            sections = [spec["template"], ", ".join(outs), ", ".join(ins), spec["clobbers"]]
            body = " : ".join(sections[:spec["colons"] + 1])
            kw = spec["keyword"] + (" " + spec["volatile"] if spec["volatile"] else "")
            text = text[:m.start()] + f"{kw}({body})" + text[close_i + 1:]
    return text


def main():
    if sys.argv[1] == "hide":
        text, specs = hide(open(sys.argv[2]).read())
        open(sys.argv[3], "w").write(text)
        json.dump(specs, open(sys.argv[4], "w"))
    elif sys.argv[1] == "restore":
        specs = json.load(open(sys.argv[3]))
        text = restore(open(sys.argv[2]).read(), specs)
        open(sys.argv[2], "w").write(text)
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
