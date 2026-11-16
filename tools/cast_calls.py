#!/usr/bin/env python3
"""Switch a file to the prototype header, keeping deviant calls as casts.

Some callers only match with a prototype that differs from the function's
definition (a different argument count, a `void` return, a narrower argument
type). Such a file can't simply include include/protos/<binary>.h, because
its own declaration would conflict with the header's. This tool removes the
file's own prototypes of header functions, includes the header, and rewrites
each call to a function whose local prototype differed as a call through an
explicit cast to that old prototype:

    void func_004403B0(void);  ...  func_004403B0(x);
becomes
    ((void (*)(void))func_004403B0)(x);

A cast changes nothing in the generated code (the caller still sees the old
prototype), but it makes the deviation visible at the call. The file is
re-checked and the change kept only if it still matches.

Run in the container: tools/docker.sh python3 tools/cast_calls.py <binary> [file ...]
With no files, it tries every file in src/<binary>/ that doesn't include the
header yet.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

PROTO_LINE = re.compile(r"^(?!static\b)(?:extern\s+)?([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}()]*)\)\s*;[ \t]*(/\*.*\*/)?[ \t]*\n", re.M)
HDR_LINE = re.compile(r"^(.+?)\b([A-Za-z_]\w*)\((.*)\);$")


def squash(s):
    return re.sub(r"\s+", "", s)


def param_types(params):
    params = re.sub(r"\s+", " ", params).strip()
    if params in ("", "void"):
        return "void"
    out = []
    for p in params.split(","):
        p = p.strip()
        m = re.match(r"^(.*?[\s\*])([A-Za-z_]\w*)$", p)
        if m and (len(p.split()) > 1 or "*" in m.group(1)):
            p = m.group(1)
        out.append(re.sub(r"\s+", " ", p).strip())
    return ", ".join(out)


def header(binary):
    protos = {}
    for b in {binary, "main"}:
        for line in open(f"include/protos/{b}.h"):
            m = HDR_LINE.match(line.strip())
            if m:
                protos[m.group(2)] = (squash(m.group(1)), squash(m.group(3)))
    return protos


def convert(text, binary, protos):
    if f'"protos/{binary}.h"' in text or '#include "common.h"' not in text:
        return None
    casts = {}
    removed = []

    def drop(m):
        ret, name, params = m.group(1).strip(), m.group(2), m.group(3)
        if name not in protos:
            return m.group(0)
        ptypes = param_types(params)
        if (squash(ret), squash(ptypes)) != protos[name]:
            casts[name] = f"({ret}(*)({ptypes}))" if ret.endswith("*") else f"({ret} (*)({ptypes}))"
        removed.append(name)
        return ""

    new = PROTO_LINE.sub(drop, text)
    if not removed:
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
    return new.replace('#include "common.h"\n', f'#include "common.h"\n#include "protos/{binary}.h"\n', 1)


def matches(path):
    r = subprocess.run(["python3", "tools/check_func.py", path], capture_output=True, text=True)
    lines = [l for l in (r.stdout + r.stderr).splitlines() if l.strip()]
    return bool(lines) and all(l.endswith(": OK") or l.startswith("rodata: OK") for l in lines)


def try_file(path, binary, protos):
    new = convert(open(path).read(), binary, protos)
    if new is None:
        return path, "skip"
    tmp = os.path.join("build", "casts_tmp", binary, os.path.basename(path))
    os.makedirs(os.path.dirname(tmp), exist_ok=True)
    with open(tmp, "w") as f:
        f.write(new)
    if matches(tmp):
        with open(path, "w") as f:
            f.write(new)
        return path, "kept"
    return path, "rejected"


def main():
    binary = sys.argv[1]
    files = sys.argv[2:]
    src = BINARIES[binary]["src"]
    if not files:
        files = sorted(os.path.join(src, f) for f in os.listdir(src) if f.endswith(".c"))
    protos = header(binary)
    with ThreadPoolExecutor(8) as ex:
        results = list(ex.map(lambda p: try_file(p, binary, protos), files))
    counts = {}
    for _, s in results:
        counts[s] = counts.get(s, 0) + 1
    with open(f"build/casts_{binary}.txt", "w") as f:
        f.writelines(f"{s} {p}\n" for p, s in results)
    print(binary, counts)


if __name__ == "__main__":
    main()
