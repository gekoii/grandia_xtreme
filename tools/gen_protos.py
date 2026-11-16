#!/usr/bin/env python3
"""Generate include/protos/<binary>.h: one prototype per function defined in C.

Each prototype is taken from the function's own definition in src/<binary>/.
Overlay headers include the main executable's, since overlays call into it.

Types: primitive types are copied as they are. A struct or union typedef from
a header under include/ (typedef struct Tag {...} Name, or an alias of one
such as GphMesh) is written as `struct Tag`, and every tag used gets a
file-scope `struct Tag;` line at the top of the header, so the protos headers
need no other header (ee-gcc 2.9 rejects a repeated typedef, so the typedef
names themselves can't be repeated here). `struct Tag *` is the same type as
the headers' `Name *`. FVECTOR/IVECTOR/FMATRIX parameters decay to `f32 *`/
`s32 *`/`f32 (*)[4]`; a function-pointer typedef parameter (MgcProc, ...) is
written out. A function is left out when its signature uses:
  - a struct passed or returned by value (GsPackST's GphFbSt),
  - a type defined only in a .c file, or one its own file defines locally
    (`} Name;` in the .c: its definition would conflict with the prototype),
  - a pointer to an array typedef (FVECTOR *), or a function-pointer return;
and the functions in EXCLUDE (a caller needs a prototype no cast can give).

The header only lists functions; files keep their own structs and globals.
tools/apply_protos.py switches files over to it one at a time and keeps the
switch only where the file still matches.

Usage: gen_protos.py [--dry-run] [binary ...]   (default: all four)
--dry-run prints what would be added, removed or changed instead of writing.
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

PRIMITIVE = {"void", "char", "int", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64",
             "f32", "f64", "float", "double", "long", "short", "unsigned", "signed", "const"}
QUALIFIERS = {"const", "volatile", "register"}
# Array typedefs (vu.h) as a parameter of that type decays to
ARRAY_PARAM = {"FVECTOR": "f32 *", "IVECTOR": "s32 *", "FMATRIX": "f32 (*)[4]"}
# Functions left out on purpose: a caller only matches with a prototype that
# can't be written as a cast (a call through a cast is never a tail call).
EXCLUDE = {
    "DbgMemDumpPage": "0x0042B100 tail-calls it as void; it returns s32",
}
DEFN = re.compile(r"^(?!static\b)([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}]*)\)\s*\{", re.M)


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return re.sub(r"//[^\n]*", "", text)


def _close_brace(text, i):
    """Index just past the brace that closes the one at text[i]."""
    depth = 0
    for j in range(i, len(text)):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                return j + 1
    return len(text)


def type_map():
    """{typedef name: 'struct Tag' / 'union Tag' / ('fn', ret, params)} from include/ (not protos/)."""
    tags, aliases, fnptrs = {}, {}, {}
    for path in glob.glob("include/**/*.h", recursive=True):
        if os.sep + "protos" + os.sep in path:
            continue
        text = strip_comments(open(path, errors="replace").read())
        for m in re.finditer(r"\btypedef\s+(struct|union)\s+(\w+)\s*\{", text):
            end = _close_brace(text, m.end() - 1)
            rest = re.match(r"\s*([^;]*);", text[end:])
            for name in (rest.group(1).split(",") if rest else []):
                name = name.strip()
                if re.fullmatch(r"[A-Za-z_]\w*", name):
                    tags[name] = f"{m.group(1)} {m.group(2)}"
        for m in re.finditer(r"\btypedef\s+(struct|union)\s+(\w+)\s+([A-Za-z_]\w*)\s*;", text):
            tags[m.group(3)] = f"{m.group(1)} {m.group(2)}"
        for m in re.finditer(r"\btypedef\s+([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*;", text):
            aliases[m.group(2)] = m.group(1)
        for m in re.finditer(r"\btypedef\s+([^;(){}]*?)\(\s*\*\s*([A-Za-z_]\w*)\s*\)\s*\(([^;{}]*)\)\s*;", text):
            fnptrs[m.group(2)] = (m.group(1), m.group(3))
    out = dict(tags)
    for name, target in aliases.items():
        seen = set()
        while target in aliases and target not in seen:
            seen.add(target)
            target = aliases[target]
        if target in tags and name not in out:
            out[name] = tags[target]
    for name, (ret, params) in fnptrs.items():
        out.setdefault(name, ("fn", ret, params))
    return out


class Converter:
    def __init__(self, types, local_names):
        self.types = types
        self.local = local_names    # typedef names the defining file declares itself
        self.tags = set()           # tags the prototypes use

    def type(self, t, param=False):
        """A parameter or return type in header form, or None if it can't be written."""
        t = re.sub(r"\s+", " ", t).strip()
        stars = t.count("*")
        words = [w for w in t.replace("*", " ").split() if w != "register"]
        if not words:
            return None
        if all(w in PRIMITIVE or w in QUALIFIERS for w in words):
            return " ".join(words) + (" " + "*" * stars if stars else "")
        quals = [w for w in words if w in QUALIFIERS]
        rest = [w for w in words if w not in QUALIFIERS]
        if rest[:1] in (["struct"], ["union"]) and len(rest) == 2:
            if not stars:
                return None                     # by value
            self.tags.add(" ".join(rest))
            return " ".join(quals + rest) + " " + "*" * stars
        if len(rest) != 1:
            return None
        name = rest[0]
        if name in self.local:
            return None
        if name in ARRAY_PARAM:
            return ARRAY_PARAM[name] if param and not stars and not quals else None
        kind = self.types.get(name)
        if kind is None:
            return None
        if isinstance(kind, tuple):             # function-pointer typedef
            if not param or stars:
                return None
            ret = self.type(kind[1])
            ps = self.params(kind[2])
            if ret is None or ps is None:
                return None
            return f"{ret} (*)({ps})".replace("* (*)", "*(*)")
        if not stars:
            return None                         # by value
        self.tags.add(kind)
        return " ".join(quals + [kind]) + " " + "*" * stars

    def params(self, params):
        params = re.sub(r"\s+", " ", params).strip()
        if params in ("", "void"):
            return "void"
        out = []
        for p in params.split(","):
            p = p.strip()
            if p == "...":
                out.append("...")
                continue
            if "(" in p:  # function pointer parameter
                return None
            # drop the parameter name (the last identifier), keep the type
            m = re.match(r"^(.*?[\s\*])([A-Za-z_]\w*)(\s*\[[^\]]*\])?$", p)
            if m and m.group(1).strip() and not (m.group(1).strip() in ("struct", "union", "const") and "*" not in m.group(1)):
                ty, arr = m.group(1), m.group(3)
            else:
                ty, arr = p, None                # no name (a typedef-only parameter in a fn typedef)
            if arr:
                ty += "*"
            ty = self.type(ty, param=True)
            if ty is None:
                return None
            out.append(ty)
        return ", ".join(out)


def local_typedefs(text):
    """Typedef names a C file defines itself."""
    names = set(re.findall(r"^\}\s*([A-Za-z_]\w*)\s*;", text, re.M))
    names |= set(re.findall(r"\btypedef\b[^;{}]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:__attribute__\s*\(\(.*?\)\)\s*)?;", text))
    names |= set(re.findall(r"\btypedef\b[^;{}]*\(\s*\*\s*([A-Za-z_]\w*)\s*\)", text))
    return names


def prototypes(binary, types=None, tags=None):
    """{function: prototype}; the struct tags they use are added to `tags`."""
    types = type_map() if types is None else types
    protos = {}
    src = BINARIES[binary]["src"]
    for name in sorted(os.listdir(src)):
        if not name.endswith(".c"):
            continue
        text = strip_comments(open(os.path.join(src, name)).read())
        conv = Converter(types, local_typedefs(text))
        for ret, fn, params in DEFN.findall(text):
            ret = re.sub(r"\b(extern|inline)\b", "", ret).strip()
            if not ret or ret in ("else", "return", "do", "while", "if") or fn in EXCLUDE:
                continue
            used = set(conv.tags)
            r = conv.type(ret)
            p = conv.params(params) if r is not None else None
            if r is None or p is None:
                conv.tags = used
                continue
            proto = f"{r} {fn}({p});".replace("* ", "*").replace(" *", " *")
            protos[fn] = (proto, conv.tags - used)
            conv.tags = used | conv.tags
    if tags is not None:
        for _, t in protos.values():
            tags |= t
    return {fn: p for fn, (p, _) in protos.items()}


def render(binary, protos, tags):
    guard = f"PROTOS_{binary.upper()}_H"
    lines = [f"/* Generated by tools/gen_protos.py from the definitions in src/{binary}/. */",
             f"#ifndef {guard}", f"#define {guard}", ""]
    if binary != "main":
        lines += ['#include "protos/main.h"', ""]
    if tags:
        lines += [f"{t};" for t in sorted(tags, key=lambda t: t.split()[1])] + [""]
    lines += [protos[k] for k in sorted(protos)]
    lines += ["", f"#endif /* {guard} */", ""]
    return "\n".join(lines)


def main():
    args = sys.argv[1:]
    dry = "--dry-run" in args
    names = [a for a in args if a != "--dry-run"] or list(BINARIES)
    types = type_map()
    os.makedirs("include/protos", exist_ok=True)
    for b in names:
        tags = set()
        protos = prototypes(b, types, tags)
        path = f"include/protos/{b}.h"
        if dry:
            cur = {}
            if os.path.exists(path):
                for line in open(path):
                    m = re.search(r"\b([A-Za-z_]\w*)\(", line)
                    if m and line.rstrip().endswith(";"):
                        cur[m.group(1)] = line.strip()
            for k in sorted(set(cur) | set(protos)):
                if k not in protos:
                    print(f"{b} - {cur[k]}")
                elif k not in cur:
                    print(f"{b} + {protos[k]}")
                elif cur[k] != protos[k]:
                    print(f"{b} ~ {cur[k]}  ->  {protos[k]}")
            continue
        with open(path, "w") as f:
            f.write(render(b, protos, tags))
        print(f"{path}: {len(protos)} prototypes, {len(tags)} struct tags")


if __name__ == "__main__":
    main()
