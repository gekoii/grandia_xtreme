#!/usr/bin/env python3
"""Collect every file's local view of each global, for merging structs.

Each C file was matched on its own and declares the globals it uses with
its own local types. For every global declared `extern` in more than one
file, this writes build/structs/<name>.txt: each declaring file, the
declared type, and the text of that type's local definition (typedef or
struct tag), so the views can be merged into one shared definition.
Also writes build/structs/index.txt: globals sorted by how many files
declare them.

Run from the repo root: python3 tools/rounds/struct_views.py [MIN_FILES]
"""
import collections
import glob
import os
import re
import sys

EXTERN = re.compile(r"^extern\s+(?:volatile\s+|const\s+)*((?:(?:struct|union|unsigned|signed)\s+)?[A-Za-z_]\w*)(?:\s+|(?=\*))(\**)\s*(?<!\w)([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)\s*;", re.M)
PRIMITIVE = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "int", "char", "void",
             "float", "double", "short", "long", "unsigned", "signed", "s128", "u128"}


def strip_comments(t):
    return re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", " ", t, flags=re.S))


def type_defs(text):
    """name -> definition text, for typedef'd structs/unions and struct tags in a file."""
    defs = {}
    for m in re.finditer(r"\b(typedef\s+)?(struct|union)\s*(\w*)\s*\{", text):
        depth, i = 0, m.end() - 1
        while i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        end = text.find(";", i)
        body = text[m.start():end + 1]
        if m.group(1):
            tm = re.search(r"\}\s*(\w+)\s*;$", body)
            if tm:
                defs[tm.group(1)] = body
        if m.group(3):
            defs[f"{m.group(2)} {m.group(3)}"] = body
    return defs


def main():
    min_files = int(sys.argv[1]) if len(sys.argv) > 1 else 2
    views = collections.defaultdict(list)
    for path in sorted(glob.glob("src/*/*.c")):
        if "pending" in path:
            continue
        text = strip_comments(open(path).read())
        defs = type_defs(text)
        for m in EXTERN.finditer(text):
            typ, ptr, name, arr = m.group(1).strip(), m.group(2), m.group(3), m.group(4) or ""
            if typ.split()[-1] in PRIMITIVE and not ptr:
                kind = "primitive"
            else:
                kind = "struct"
            body = defs.get(typ) or defs.get(typ.split()[-1]) or ""
            views[name].append((path, f"{typ} {ptr}{name}{arr}".strip(), kind, body))
    os.makedirs("build/structs", exist_ok=True)
    ranked = sorted(((len({p for p, *_ in v}), n) for n, v in views.items()), reverse=True)
    with open("build/structs/index.txt", "w") as idx:
        for count, name in ranked:
            if count < min_files:
                continue
            v = views[name]
            types = {decl.split(name)[0].strip() for _, decl, _, _ in v}
            idx.write(f"{count:5d} files  {len(types):3d} types  {name}\n")
            with open(f"build/structs/{name}.txt", "w") as f:
                for path, decl, kind, body in v:
                    f.write(f"== {path}: {decl}\n")
                    if body:
                        f.write(body + "\n")
    print(f"{sum(1 for c, _ in ranked if c >= min_files)} globals declared in {min_files}+ files")


if __name__ == "__main__":
    main()
