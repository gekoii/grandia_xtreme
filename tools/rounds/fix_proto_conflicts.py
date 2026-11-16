#!/usr/bin/env python3
"""Fix files whose local prototypes conflict with the regenerated headers.

After include/protos/<binary>.h gains functions (or a function's prototype
changes), a file that already includes it may still declare one of them
locally with a different signature (a compile error), or call one it never
declared. For each FILE this removes the file's own prototypes of header
functions and re-checks the file, trying in turn:

  1. no casts at all (the calls go through the header's prototypes);
  2. no casts where the old and new prototypes differ only in what their
     pointers point to (`void *`, a local view, `struct Tag *`: the same code),
     casts to the old prototype where they differ otherwise;
  3. tools/cast_calls.py's conversion: every call whose local prototype
     differed in any way goes through a cast to it.

The first that matches is kept; otherwise the file is left as it was.
Run in the container: fix_proto_conflicts.py [--modes none,loose,strict] BINARY FILE...
(--modes none,loose also tidies files that already match: their local
prototypes of header functions go where no cast, or only a needed one, is
the price.)
"""
import os
import re
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
import cast_calls  # noqa: E402

POINTERISH = re.compile(r"\*|\(\*\)|^(FVECTOR|IVECTOR|FMATRIX)$")


def split_params(p):
    out, depth, cur = [], 0, ""
    for c in p:
        if c == "," and depth == 0:
            out.append(cur)
            cur = ""
            continue
        depth += c == "("
        depth -= c == ")"
        cur += c
    out.append(cur)
    return out


def same_code(old, new, fnptr_types):
    """True if two squashed types differ at most in what a pointer points to."""
    if old == new:
        return True
    ptr = lambda t: bool(POINTERISH.search(t)) or t in fnptr_types  # noqa: E731
    return ptr(old) and ptr(new)


def convert(text, binary, protos, mode, fnptr_types):
    """mode: "none", "loose" or "strict" (see above)."""
    inc = f'#include "protos/{binary}.h"\n'
    had_inc = inc in text
    text = text.replace(inc, "")
    if '#include "common.h"' not in text:
        return None
    casts = {}
    removed = []

    def drop(m):
        ret, name, params = m.group(1).strip(), m.group(2), m.group(3)
        if name not in protos:
            return m.group(0)
        ptypes = cast_calls.param_types(params)
        old = (cast_calls.squash(ret), cast_calls.squash(ptypes))
        if old != protos[name] and mode != "none":
            strict = mode == "strict"
            hret, hparams = protos[name]
            op, hp = split_params(old[1]), split_params(hparams)
            differs = strict or len(op) != len(hp) or not same_code(old[0], hret, fnptr_types) \
                or not all(same_code(a, b, fnptr_types) for a, b in zip(op, hp))
            if differs:
                casts[name] = f"({ret}(*)({ptypes}))" if ret.endswith("*") else f"({ret} (*)({ptypes}))"
        removed.append(name)
        return ""

    new = cast_calls.PROTO_LINE.sub(drop, text)
    if not removed and had_inc:
        return None
    for name, cast in casts.items():
        # every use must be a call, or the cast would be wrong
        uses = re.findall(r"\b" + name + r"\b\s*(.)", new)
        if any(c != "(" for c in uses):
            return None
        # leave the definition (if the file has it) alone
        if re.search(r"^[A-Za-z_][\w \t\*]*?\b" + name + r"\s*\([^;{}()]*\)\s*\{", new, re.M):
            return None
        new = re.sub(r"\b" + name + r"\s*\(", f"({cast}{name})(", new)
    return new.replace('#include "common.h"\n', '#include "common.h"\n' + inc, 1)


def fnptr_typedefs():
    names = set()
    for root, _, files in os.walk("include"):
        for f in files:
            if f.endswith(".h"):
                names |= set(re.findall(r"\btypedef\b[^;{}]*\(\s*\*\s*([A-Za-z_]\w*)\s*\)", open(os.path.join(root, f), errors="replace").read()))
    return names


def main():
    args = sys.argv[1:]
    modes = ("none", "loose", "strict")
    if args[:1] == ["--modes"]:
        modes, args = tuple(args[1].split(",")), args[2:]
    binary, files = args[0], " ".join(args[1:]).split()
    protos = cast_calls.header(binary)
    fnptrs = fnptr_typedefs()
    for path in files:
        text = open(path).read()
        tmp = os.path.join("build", "casts_tmp", "fix_" + path.replace("/", "_"))
        os.makedirs(os.path.dirname(tmp), exist_ok=True)
        done = None
        for mode in modes:
            new = convert(text, binary, protos, mode, fnptrs)
            if new is None:
                break
            open(tmp, "w").write(new)
            if cast_calls.matches(tmp):
                done = new
                break
        if done is None:
            print("still failing" if new is not None else "skip", path, flush=True)
            continue
        open(path, "w").write(done)
        print("fixed" + ("" if mode == "none" else f" ({mode} casts)"), path, flush=True)


if __name__ == "__main__":
    main()
