#!/usr/bin/env python3
"""Split the .data/.rodata that is still asm into data-only C files and
batches of work for parallel agents (plan step 6b; see
docs/notes/data_step6b_rules.md).

Usage: tools/rounds/plan_data_tasks.py [--out build/data/step6b_tasks.json] [--batches N]
                                       [--check]

From the current split (asm/, the splat configs) it:

  1. takes every asm .data/.rodata piece of the four binaries (not main's
     .text_nop), leaving out the jump tables of asm functions (they go with
     their function) and runs of zeros with no label (padding);
  2. classifies each label by the code that uses it: Sony SDK (main's
     libgraph..libpad 0x0046CF80-0x0047ABC0 and kernel/sif/fileio from
     0x0048BC40) stays asm and is left out; newlib/libm (0x0047ABC0-
     0x0048BC40) is kept apart and marked; the rest is game data. A label
     only data refers to takes the class of the label before it;
  3. cuts the pieces into files: .rodata at each "save" string (the start of
     an original file's .rodata), .data where the first user's C file
     changes, merging small neighbours (same class) up to MAX_FILE bytes.
     A cut is always at an 8-aligned label, where any object can start;
  4. groups the files into batches of about equal work (bytes, pointer
     words, labels), each within one binary and in address order.

With --check it also drafts every file with tools/gen_data_c.py into
build/data/scratch_6b/drafts/ (run it in the container: it checks each draft
with tools/check_data.py) and records whether the draft passes as it is.
"""
import argparse
import collections
import glob
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from binaries import BINARIES  # noqa: E402
from data_asm import ROOT, asm_pieces, config_pieces, rom  # noqa: E402
import gen_data_c  # noqa: E402

SONY = [(0x0046CF80, 0x0047ABC0), (0x0048BC40, 0x00495A60)]
NEWLIB = [(0x0047ABC0, 0x0048BC40)]
MAX_FILE = 0x1000
MIN_FILE = 0x200
TOKEN_RE = re.compile(r"[A-Za-z_.$][\w.$]*")


def code_class(b, addr):
    if b != "main" or addr is None:
        return "game"
    if any(lo <= addr < hi for lo, hi in SONY):
        return "sony"
    if any(lo <= addr < hi for lo, hi in NEWLIB):
        return "newlib"
    return "game"


def users(b, names):
    """{label: [user addresses]}: the C files (by start address) and asm
    functions of the binary that mention each label."""
    out = collections.defaultdict(set)
    for path in glob.glob(os.path.join(ROOT, f"src/{b}/*.c")):
        stem = os.path.basename(path)[:-2]
        if not re.fullmatch(r"[0-9A-F]{6}", stem):
            continue
        for tok in set(TOKEN_RE.findall(open(path).read())) & names:
            out[tok].add(int(stem, 16))
    for path in glob.glob(os.path.join(ROOT, f"asm/{b}/{b}/*.s")):
        func = None
        for line in open(path):
            m = re.match(r"\s*glabel\s+(\S+)", line)
            if m:
                func = None
                continue
            if func is None:
                m = re.search(r"/\*\s+[0-9A-F]+\s+([0-9A-F]{8})\s", line)
                if m:
                    func = int(m.group(1), 16)
            for tok in set(TOKEN_RE.findall(line.split("*/")[-1])) & names:
                out[tok].add(func)
    return out


def c_file_of(b, addr, c_starts):
    """Group key of a user: the C file (or asm stretch) containing addr."""
    import bisect
    i = bisect.bisect_right(c_starts, addr) - 1
    return c_starts[i] if i >= 0 else None


def jump_tables(b):
    path = os.path.join(ROOT, BINARIES[b]["jtbls"])
    out = []
    for line in open(path):
        if line.strip() and not line.startswith("#"):
            _, addr, size = line.split()
            out.append((int(addr, 16), int(size, 16)))
    return out


def plan_binary(b, image):
    vram = BINARIES[b]["vram"]
    pieces = [p for p in asm_pieces(b) if not (b == "main" and p.start == 0x00499A70)]
    names = set()
    for p in pieces:
        for v, ns in p.labels.items():
            names.update(ns)
    use = users(b, names)
    c_starts = sorted(s for s, e, typ, name in config_pieces(b) if typ in ("c", "asm"))
    tables = jump_tables(b)
    units = []
    for p in pieces:
        # label -> (address, first user, class)
        labels = sorted((v, ns[0]) for v, ns in p.labels.items() if p.start <= v < p.end)
        info = {}
        for v, n in labels:
            us = [u for ns in p.labels[v] for u in use.get(ns, ()) if u is not None]
            first = min(us) if us else None
            cls = code_class(b, first) if first is not None else None
            info[v] = [n, first, cls, c_file_of(b, first, c_starts) if first is not None else None]
        # labels only data refers to: the class of the label before (else after)
        known = [v for v in sorted(info) if info[v][2]]
        for v in sorted(info):
            if info[v][2] is None:
                before = [k for k in known if k < v]
                after = [k for k in known if k > v]
                info[v][2] = info[before[-1]][2] if before else info[after[0]][2] if after else "game"
        # segments of the piece without jump tables
        cuts = [(p.start, p.end)]
        for t, size in tables:
            new = []
            for s, e in cuts:
                if s <= t < e:
                    if t > s:
                        new.append((s, t))
                    if t + size < e:
                        new.append((t + size, e))
                else:
                    new.append((s, e))
            cuts = new
        for s, e in cuts:
            units += split_segment(b, p, s, e, info, image, vram)
    return units


def quiet(f, *args):
    import contextlib
    import io
    with contextlib.redirect_stderr(io.StringIO()):
        return f(*args)


def split_segment(b, p, s, e, info, image, vram):
    """Cut [s, e) of piece p into files."""
    data = image[s - vram:e - vram]
    if not any(data) and not any(s <= v < e for v in info):
        return []
    # A file's section is aligned to its most aligned object, 8 as soon as it
    # has an array, struct or string (DATA_ALIGNMENT), so a file starts at an
    # 8-aligned element. What comes before stays asm (usually the zero
    # padding before a new file's .rodata, which the alignment recreates).
    try:
        elems, labels, sec = gen_data_c.elements(b, s, e, image, None)
    except SystemExit:
        return []
    starts = [x.addr for x in elems if x.addr % 8 == 0 and x.kind != "pad"]
    if not starts:
        return []
    start = starts[0]
    # candidate cuts: 8-aligned labels (and unlabelled "save" strings in .rodata)
    cand = {}
    for v in sorted(info):
        if start < v < e and v % 8 == 0:
            cand[v] = info[v]
    saves = set()
    if p.section == ".rodata":
        # the header's "save" string: the start of an original file's .rodata
        for a in range(start + (-start % 8), e - 4, 8):
            if image[a - vram:a - vram + 5] == b"save\0" and \
                    (a == start or not gen_data_c.texty(image, a - 1 - vram)):
                saves.add(a)
                if a > start:
                    cand.setdefault(a, ("(save)", None, None, None))
    bounds = [start]
    cur_key = None
    cur_class = None
    for v in sorted(cand):
        n, first, cls, key = cand[v]
        cls = cls or cur_class
        if v in saves and p.section == ".rodata":
            bounds.append(v)
        elif cls != cur_class and cur_class is not None:
            bounds.append(v)
        elif p.section == ".data" and key is not None and cur_key is not None and key != cur_key:
            bounds.append(v)
        if key is not None:
            cur_key = key
        cur_class = cls
    bounds = sorted(set(bounds)) + [e]
    raw = []
    for x, y in zip(bounds, bounds[1:]):
        # the class of the code using it (labels code refers to count first)
        counts = collections.Counter(info[v][2] for v in info if x <= v < y and info[v][1] is not None)
        if not counts:
            counts = collections.Counter(info[v][2] for v in info if x <= v < y)
        if counts:
            cls = counts.most_common(1)[0][0]
        else:
            prev = [info[v][2] for v in sorted(info) if v < x]
            cls = prev[-1] if prev else "game"
        raw.append([x, y, cls])
    # merge small neighbours of the same class, up to MAX_FILE
    merged = []
    for u in raw:
        if merged and merged[-1][2] == u[2] and (u[1] - merged[-1][0] <= MAX_FILE) and \
                (u[1] - u[0] < MIN_FILE or merged[-1][1] - merged[-1][0] < MIN_FILE):
            merged[-1][1] = u[1]
        else:
            merged.append(u)
    # split files that are still too big at 16- (else 8-) aligned labels
    out = []
    for x, y, cls in merged:
        while y - x > MAX_FILE * 2:
            cut = [v for v in sorted(cand) if x + MAX_FILE <= v < y - MIN_FILE and v % 16 == 0] or \
                  [v for v in sorted(cand) if x + MAX_FILE <= v < y - MIN_FILE]
            if not cut:
                break
            out.append([x, cut[0], cls])
            x = cut[0]
        out.append([x, y, cls])
    units = []
    for x, y, cls in out:
        d = image[x - vram:y - vram]
        if not any(d) and not any(x <= v < y for v in info):
            continue
        sym = sum(1 for v in p.symbolic_words() if x <= v < y)
        nl = sum(1 for v in info if x <= v < y)
        users_ = sorted({f"{info[v][1]:08X}" for v in info if x <= v < y and info[v][1] is not None})
        units.append(dict(binary=b, section=p.section, start=x, end=y, size=y - x, ptr_words=sym,
                          labels=nl, saves=sum(1 for a in saves if x <= a < y), library=cls,
                          asm_piece=os.path.relpath(p.path, ROOT), first_users=users_[:6]))
    return units


def work(u):
    """Rough effort: typing the bytes (big byte/halfword tables are quick per
    byte), each pointer and each label (an object to type or a field to
    place), and a fixed cost per file."""
    return u["size"] // 4 + 24 * u["ptr_words"] + 16 * u["labels"] + 64


def stem(u):
    return f"{u['start']:06X}" if u["start"] < 0x01000000 else f"{u['start']:08X}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default="build/data/step6b_tasks.json")
    ap.add_argument("--batches", type=int, default=28)
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()
    os.chdir(ROOT)
    units = []
    for b in BINARIES:
        units += plan_binary(b, rom(b))
    for u in units:
        u["c_file"] = f"src/{u['binary']}/data/{stem(u)}.c"
        u["subsegment"] = (f"[0x{u['start'] - BINARIES[u['binary']]['vram']:06X}, {u['section']}, "
                           f"{u['binary']}/data/{stem(u)}]")

    if a.check:
        draft_dir = "build/data/scratch_6b/drafts"
        import shutil
        shutil.rmtree(os.path.join(ROOT, "build/data/scratch_6b/drafts"), ignore_errors=True)
        os.makedirs(draft_dir, exist_ok=True)
        jobs = []
        for u in units:
            if u["library"] == "sony":
                continue
            path = f"{draft_dir}/{u['binary']}/{stem(u)}.c"
            os.makedirs(os.path.dirname(path), exist_ok=True)
            try:
                open(path, "w").write(gen_data_c.generate(u["binary"], u["start"], u["end"]))
            except SystemExit as e:
                u["draft"] = f"generator failed: {e}"
                continue
            jobs.append((u, path))
        from concurrent.futures import ThreadPoolExecutor

        def chk(job):
            u, path = job
            r = subprocess.run([sys.executable, "tools/check_data.py", path], capture_output=True, text=True)
            return u, r.returncode, (r.stdout + r.stderr).strip().splitlines()

        with ThreadPoolExecutor(8) as ex:
            for u, rc, out in ex.map(chk, jobs):
                u["draft"] = "OK" if rc == 0 else "FAILED: " + "; ".join(l for l in out if l.startswith("error"))[:300]

    todo = [u for u in units if u["library"] != "sony"]
    sony = [u for u in units if u["library"] == "sony"]
    total = sum(work(u) for u in todo)
    target = total / a.batches
    batches = []
    for b in BINARIES:
        for lib in ("game", "newlib"):
            cur = []
            for u in [u for u in todo if u["binary"] == b and u["library"] == lib]:
                if cur and sum(work(x) for x in cur) + work(u) / 2 > target:
                    batches.append(cur)
                    cur = []
                cur.append(u)
            if cur:
                # a small remainder joins the binary's previous batch
                if batches and batches[-1][0]["binary"] == b and batches[-1][0]["library"] == lib and \
                        sum(work(x) for x in cur) < target * 0.4:
                    batches[-1] += cur
                else:
                    batches.append(cur)
    out = {
        "generated_by": "tools/rounds/plan_data_tasks.py",
        "rules": "docs/notes/data_step6b_rules.md",
        "apply": ("Copy every batch's C files to src/<binary>/data/, run tools/update_segments.py for each "
                  "binary (it writes the .data/.rodata subsegments from the files' markers, so all batches "
                  "apply in one run), then tools/split.sh, make, tools/check.sh, check_syms, shift_test."),
        "totals": {
            "files": len(todo), "batches": len(batches), "bytes": sum(u["size"] for u in todo),
            "ptr_words": sum(u["ptr_words"] for u in todo), "labels": sum(u["labels"] for u in todo),
            "newlib_files": sum(1 for u in todo if u["library"] == "newlib"),
            "newlib_bytes": sum(u["size"] for u in todo if u["library"] == "newlib"),
            "sony_left_asm": {"files": len(sony), "bytes": sum(u["size"] for u in sony)},
            "drafts_ok": sum(1 for u in todo if u.get("draft") == "OK") if a.check else None,
        },
        "batches": [],
        "sony_left_asm": [dict(binary=u["binary"], section=u["section"], range=f"0x{u['start']:08X}-0x{u['end']:08X}",
                               size=u["size"], asm_piece=u["asm_piece"]) for u in sony],
    }
    for i, bt in enumerate(batches):
        out["batches"].append({
            "id": f"6b-{i + 1:02d}",
            "binary": bt[0]["binary"],
            "library": bt[0]["library"],
            "size": sum(u["size"] for u in bt),
            "ptr_words": sum(u["ptr_words"] for u in bt),
            "labels": sum(u["labels"] for u in bt),
            "work": sum(work(u) for u in bt),
            "asm_pieces": sorted({u["asm_piece"] for u in bt}),
            "work_dir": f"build/data/step6b/6b-{i + 1:02d}/",
            "files": [dict(c_file=u["c_file"], work_file=f"build/data/step6b/6b-{i + 1:02d}/{stem(u)}.c",
                           section=u["section"],
                           range=f"0x{u['start']:08X}-0x{u['end']:08X}", size=u["size"],
                           ptr_words=u["ptr_words"], labels=u["labels"], saves=u["saves"],
                           first_users=u["first_users"], draft=u.get("draft"),
                           subsegment=u["subsegment"]) for u in bt],
            "subsegments": [u["subsegment"] for u in bt],
        })
    os.makedirs(os.path.dirname(a.out), exist_ok=True)
    json.dump(out, open(a.out, "w"), indent=1)
    t = out["totals"]
    print(f"{t['files']} files in {t['batches']} batches, {t['bytes']} bytes, {t['ptr_words']} pointer words, "
          f"{t['labels']} labels; newlib {t['newlib_files']} files / {t['newlib_bytes']} bytes; "
          f"Sony left asm: {t['sony_left_asm']}; drafts OK: {t['drafts_ok']}")
    for bt in out["batches"]:
        print(f"  {bt['id']} {bt['binary']:6} {bt['library']:6} {len(bt['files']):3} files {bt['size']:6} B "
              f"{bt['ptr_words']:4} ptrs {bt['labels']:4} labels work {bt['work']}")


if __name__ == "__main__":
    main()
