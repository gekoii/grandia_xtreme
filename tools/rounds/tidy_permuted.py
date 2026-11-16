#!/usr/bin/env python3
"""Turn permuter matches back into normal source files and re-check them.

Reads the matched addresses from build/agents/permute_matched.json, takes each
build/permuter/ADDR/match.c (preprocessed), restores the #include lines in
place of the pasted header contents, re-applies the toolchain markers, and
re-checks the result with check_func.py. Passing files go to
build/permuter/final/ADDR.c.

    tools/docker.sh python3 tools/rounds/tidy_permuted.py
"""
import json, os, re, subprocess
matched = json.load(open("build/agents/permute_matched.json"))
pick = {"%08X" % x[0]: x[1] for x in json.load(open("build/agents/permute_pick.json"))}
def pre(h):
    return subprocess.run(["tools/compilers/ee-gcc2.9-991111/bin/ee-gcc", "-E", "-P", "-Iinclude", h],
                          capture_output=True, text=True).stdout.splitlines()
hdr = {"common.h": set(l for l in pre("include/common.h") if l.strip()),
       "m2c_macros.h": set(l for l in pre("include/m2c_macros.h") if l.strip())}
os.makedirs("build/permuter/final", exist_ok=True)
import glob
done_src = " ".join(open(p).read() for p in glob.glob("src/main/*.c"))
for a in matched:
    d = f"build/permuter/{a}"
    if f"func_{a}(" in done_src and re.search(r"func_%s\(.*\)\s*\{" % a, done_src):
        print(a, "SKIP already in src/main"); continue
    if a in pick:
        src = open(pick[a]).read()
    else:
        # Original attempt unknown: include what the permuter output needs
        body = open(f"{d}/match.c").read()
        src = '#include "common.h"\n' + ('#include "m2c_macros.h"\n' if "M2C_" in body else "")
    markers = open(f"{d}/markers.h").read()
    body = open(f"{d}/match.c").read().splitlines()
    incs = [h for h in ("common.h", "m2c_macros.h") if f'#include "{h}"' in src]
    drop = set().union(*(hdr[h] for h in incs)) if incs else set()
    kept = [l for l in body if l not in drop]
    tidy = markers + "".join(f'#include "{h}"\n' for h in incs) + "\n" + "\n".join(kept).strip() + "\n"
    out = f"build/permuter/final/{a}.c"
    for attempt, text in (("tidy", tidy), ("raw", markers + "\n".join(body) + "\n")):
        open(out, "w").write(text)
        r = subprocess.run(["python3", "tools/check_func.py", out], capture_output=True, text=True)
        if r.returncode == 0:
            print(a, "OK", attempt); break
    else:
        print(a, "FAIL", r.stdout.splitlines()[:1]); os.remove(out)
