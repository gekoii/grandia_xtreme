#!/usr/bin/env python3
"""Score every saved attempt from agent rounds and keep the best per function.

Scans build/agents/scratch_r*/ and build/agents/fixes*/ for single-function
C files of functions not yet in src/main/, checks each with check_func.py,
and copies the closest attempt per function to build/agents/best/ADDR.c
(scores in build/agents/best_scores.json). Also writes
build/agents/permute_pick.json, the near misses worth giving the permuter.

Run inside the build container from the repo root:
    tools/docker.sh python3 tools/rounds/eval_nearmiss.py
"""
import glob, json, os, re, subprocess, sys
from multiprocessing import Pool
sys.path.insert(0, "tools")
from check_func import defined_functions
ASM_RE = re.compile(r'\b(?:__asm__|asm)\s*(?:volatile\s*)?\(\s*((?:"(?:[^"\\]|\\.)*"\s*)+)')
# Inline asm the project allows (docs/toolchain.md): sync, libgcc's multu,
# and lq/sq quadword copies and loads, at most two instructions per statement.
ALLOWED_OPS = {"sync", "sync.l", "multu", "lq", "sq", "sqrt.s", "min.s", "max.s"}


def is_vu0(op):
    return op in ("lqc2", "sqc2", "vcallms", "vcallmsr") or op.startswith(("v", "qmfc2", "qmtc2", "cfc2", "ctc2"))


def asm_allowed(text):
    for strings in ASM_RE.findall(text):
        body = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', strings))
        insns = [l.split()[0] for l in re.split(r"\\n|;|\n", body.replace("\\t", " ")) if l.split()]
        if insns and all(is_vu0(o) for o in insns):
            continue  # a VU0 macro statement (docs/toolchain.md)
        if not set(insns) <= ALLOWED_OPS or len(insns) > 2:
            return False
    return True
done = set()
for p in glob.glob("src/main/*.c"):
    done |= {a for _, a in defined_functions(open(p).read())}
jt = set()  # jump-table functions are fine now
files = glob.glob("build/agents/scratch_r*/**/*.c", recursive=True) + glob.glob("build/agents/fixes*/*.c")
cands = []
for p in files:
    try: t = open(p).read()
    except OSError: continue
    if not asm_allowed(t): continue
    fs = defined_functions(t)
    if len(fs) != 1 or fs[0][1] in done: continue
    if not os.path.exists("build/m2c/main/%08X.s" % fs[0][1]): continue
    cands.append((fs[0][1], p))
def score(item):
    a, p = item
    r = subprocess.run(["python3", "tools/check_func.py", p], capture_output=True, text=True)
    m = re.search(r"(\d+) differing words \(compiled (\d+) insns, original (\d+)\)", r.stdout)
    return (a, p, int(m.group(1)), int(m.group(2)), int(m.group(3))) if m else None
with Pool(16) as pool:
    res = [x for x in pool.map(score, cands) if x]
best = {}
for a, p, d, g, w in res:
    k = (abs(g - w), d)
    if a not in best or k < best[a][0]: best[a] = (k, p, d, g, w)
import shutil
os.makedirs("build/agents/best", exist_ok=True)
for a, v in best.items():
    shutil.copy(v[1], "build/agents/best/%08X.c" % a)
json.dump({"%08X" % a: {"path": v[1], "diffs": v[2], "got": v[3], "want": v[4]} for a, v in best.items()},
          open("build/agents/best_scores.json", "w"), indent=1)
pick = [(a, v[1], v[2], v[3], v[4]) for a, v in best.items()
        if (v[3] == v[4] and v[2] <= 60) or (abs(v[3] - v[4]) <= 2 and v[2] <= 30)]
pick.sort(key=lambda x: (abs(x[3] - x[4]), x[2]))
json.dump(pick, open("build/agents/permute_pick.json", "w"), indent=1)
print(len(cands), "attempt files,", len(best), "functions scored,", len(pick), "picked for the permuter")
print("diff counts:", [x[2] for x in pick])
