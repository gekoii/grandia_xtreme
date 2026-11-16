#!/usr/bin/env python3
"""What stands in the way of a 64-bit build: compile the game's C (as
build.py generates it) for x86_64-w64-mingw32, syntax only, with the
warnings that mark 32-bit assumptions, and count them.

    python port/tools/w64_survey.py [-j N] [--out DIR] [FILE...]

Writes DIR/warnings.txt (every diagnostic), DIR/by_flag.txt and
DIR/by_file.tsv. Nothing is linked, and build/port is left alone.
"""

import argparse
import collections
import concurrent.futures
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
import build  # noqa: E402

TARGET64 = "x86_64-w64-mingw32"
WARNINGS = [
    "-Wno-everything",
    "-Wint-to-pointer-cast", "-Wpointer-to-int-cast", "-Wvoid-pointer-to-int-cast",
    "-Wint-conversion", "-Wincompatible-pointer-types", "-Wincompatible-function-pointer-types",
    "-Wshorten-64-to-32",
]
DIAG = re.compile(r"^(?P<file>[^:\n]+):(?P<line>\d+):\d+: (?P<kind>warning|error): (?P<msg>.*?)(?: \[(?P<flag>-W[\w-]+)[^\]]*\])?$",
                  re.M)


def survey_one(clang, rel, out_dir):
    src = build.generate(rel, out_dir=os.path.join(out_dir, "gen"))
    flags = [f for f in build.game_flags(rel, "1") if f != "-w"] + WARNINGS
    p = subprocess.run([clang, "-target", TARGET64, "-fsyntax-only", "-ferror-limit=0", src] + flags,
                       capture_output=True, text=True, cwd=build.ROOT)
    return rel, p.returncode, p.stderr


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count())
    ap.add_argument("--out", default=os.path.join(build.ROOT, "build", "skip", "w64"))
    ap.add_argument("files", nargs="*")
    args = ap.parse_args()
    out = os.path.abspath(args.out)
    os.makedirs(out, exist_ok=True)
    build.set_target("x86_64")  # its copy of include/ (build/port64/hdr)
    clang = build.direct_clang(build.find_clang())
    if args.files:
        rels = [os.path.relpath(os.path.abspath(f), build.ROOT).replace("\\", "/") for f in args.files]
    else:
        rels = [rel for rel, _, host in build.game_sources() if not host]

    by_flag = collections.Counter()
    by_file = collections.defaultdict(collections.Counter)
    failed = []
    with open(os.path.join(out, "warnings.txt"), "w", encoding="utf-8") as log, \
            concurrent.futures.ThreadPoolExecutor(args.jobs) as ex:
        for rel, rc, err in ex.map(lambda r: survey_one(clang, r, out), rels):
            if rc:
                failed.append(rel)
            if err:
                log.write(f"=== {rel}\n{err}\n")
            for m in DIAG.finditer(err):
                key = m.group("flag") or ("error: " + re.sub(r"'[^']*'", "'…'", m.group("msg"))[:80]
                                          if m.group("kind") == "error" else "other")
                by_flag[key] += 1
                by_file[rel][key] += 1

    with open(os.path.join(out, "by_flag.txt"), "w", encoding="utf-8") as f:
        for k, n in by_flag.most_common():
            f.write(f"{n:7d} {k}\n")
    with open(os.path.join(out, "by_file.tsv"), "w", encoding="utf-8") as f:
        for rel in sorted(by_file, key=lambda r: -sum(by_file[r].values())):
            f.write(f"{rel}\t{sum(by_file[rel].values())}\t"
                    + " ".join(f"{k}={n}" for k, n in by_file[rel].most_common()) + "\n")
    print(f"{len(rels)} files, {len(failed)} with errors, {sum(by_flag.values())} diagnostics, "
          f"{len(by_file)} files with any")
    for k, n in by_flag.most_common(25):
        print(f"{n:7d} {k}")


if __name__ == "__main__":
    main()
