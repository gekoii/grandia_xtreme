#!/usr/bin/env python3
"""Retry saved near-miss attempts under every compiler/assembler/flags marker.

For each function not yet in src/main/, takes up to 6 recent attempts from
the scratch folders listed below and re-checks each with every marker
combination in COMBOS. Matches are written to build/agents/retry_tc/ and
listed in build/agents/retry_tc.json. ROUND picks the round: its scratch
folders (build/agents/scratch_rROUND_*) are the attempts, and its fixes
folder counts as done.

    tools/docker.sh python3 tools/rounds/retry_tc.py ROUND
"""
import glob, json, os, re, subprocess, sys
from multiprocessing import Pool
sys.path.insert(0, "tools")
from check_func import defined_functions

COMBOS = [
    ("cc01", "/* cc: ee-gcc2.9-991111-01 */\n"),
    ("as11", "/* as: ee-gcc2.9-991111 */\n"),
    ("cc01as01", "/* cc: ee-gcc2.9-991111-01 */\n/* as: ee-gcc2.9-991111-01 */\n"),
    ("cc01as11", "/* cc: ee-gcc2.9-991111-01 */\n/* as: ee-gcc2.9-991111 */\n"),
    ("as01", "/* as: ee-gcc2.9-991111-01 */\n"),
    ("nogcse", "/* cflags: -fno-gcse */\n"),
    ("nosched", "/* cflags: -fno-schedule-insns */\n"),
    ("nodb", "/* cflags: -fno-delayed-branch */\n"),
    ("cc01nodb", "/* cc: ee-gcc2.9-991111-01 */\n/* cflags: -fno-delayed-branch */\n"),
    ("as11a", "/* as: ee-gcc2.9-991111a */\n"),
    ("as0721", "/* as: ee-gcc2.9-990721 */\n"),
    ("cc01as11a", "/* cc: ee-gcc2.9-991111-01 */\n/* as: ee-gcc2.9-991111a */\n"),
    ("cc01as0721", "/* cc: ee-gcc2.9-991111-01 */\n/* as: ee-gcc2.9-990721 */\n"),
]
ASM_RE = re.compile(r'\b(__asm__|asm)\s*(volatile\s*)?\(')
SYNC_RE = re.compile(r'\b(__asm__|asm)\s*(volatile\s*)?\(\s*"sync(\.l)?"\s*\)')

ROUND = sys.argv[1] if len(sys.argv) > 1 else "7"

done = set()
for p in glob.glob("src/main/*.c") + glob.glob(f"build/agents/fixes{ROUND}/*.c"):
    done |= {a for _, a in defined_functions(open(p).read())}

cands = {}
paths = [p for p in glob.glob(f"build/agents/scratch_r{ROUND}_*/**/*.c", recursive=True)]
for p in paths:
    try:
        t = open(p).read()
    except OSError:
        continue
    if len(ASM_RE.findall(t)) != len(SYNC_RE.findall(t)):
        continue
    fs = defined_functions(t)
    if len(fs) != 1 or fs[0][1] in done:
        continue
    cands.setdefault(fs[0][1], []).append(p)
# Most recent attempts first, at most 6 per function
for a in cands:
    cands[a] = sorted(set(cands[a]), key=lambda p: -os.path.getmtime(p))[:6]

def try_func(item):
    addr, files = item
    for f in files:
        body = open(f).read()
        for tag, marker in COMBOS:
            out = f"build/agents/retry_tc/{addr:08X}_{tag}.c"
            open(out, "w").write(marker + body)
            r = subprocess.run(["python3", "tools/check_func.py", out], capture_output=True, text=True)
            if r.returncode == 0:
                return (addr, out)
            os.remove(out)
    return (addr, None)

if __name__ == "__main__":
    with Pool(16) as pool:
        res = pool.map(try_func, sorted(cands.items()))
    ok = [(a, o) for a, o in res if o]
    print(len(cands), "functions retried,", sum(len(v) for v in cands.values()), "attempt files;", len(ok), "now match")
    json.dump({"%08X" % a: o for a, o in ok}, open("build/agents/retry_tc.json", "w"), indent=1)
    for a, o in ok: print("%08X" % a, o)
