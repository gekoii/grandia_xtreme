#!/usr/bin/env python3
"""Replace files in a copy of the game's disc image (ISO9660), for run-time
tests of rebuilt binaries in an emulator.

Usage:
  python3 tools/iso_patch.py --out build/iso/NAME.iso --dir build/boot/NAME
  python3 tools/iso_patch.py --out OUT.iso SLUS_204.17=path FIELD.BIN=path ...
      [--src "disc/Grandia Xtreme.ISO"] [--rar "Grandia Xtreme (USA).rar"]

  --dir    replace SLUS_204.17, FIELD.BIN, BATTLE.BIN and VIEWER.BIN with
           the files of that name in DIR (tools/boot_build.py writes them)
  --src    the original image (default: disc/Grandia Xtreme.ISO, the
           user's own copy, extracted from the .rar in the repo root; the
           macOS tools here can't unpack that .rar, so it is checked against
           the CRC-32 the .rar records instead, see --rar)
  --rar    check --src against this .rar's CRC-32 for it first (default:
           "Grandia Xtreme (USA).rar" if present); --no-rar-check skips it

How: the image is copied (an APFS clone where possible) and every replaced
file is appended at the end of the copy, sector aligned, with its ISO9660
directory record(s) rewritten to point there (extent and size, both byte
orders) and the volume size in the volume descriptors raised. Nothing else
changes: the old copies stay where they were, unreferenced. Appending
avoids the question of sector slack (FIELD.BIN has 442 bytes); rewriting the
directory records matters because the IOP's CD/DVD driver finds files
through them (sceCdSearchFile, and the sound driver's own lookup). The
disc's UDF bridge isn't updated: the PS2 and PCSX2 read ISO9660 only.

Then the result is read back with a fresh ISO9660 parse: every replaced
file must read back byte for byte as its input, every other file must be
identical to the source image's, and the sectors that differ from the
source within its length are listed (they must be the volume descriptors
and the directory sectors holding the rewritten records).

The images contain the game: they stay under build/ (gitignored) or outside
the repository.
"""
import argparse
import os
import shutil
import struct
import subprocess
import sys
import zlib

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SECTOR = 2048
DEFAULT_SRC = os.path.join(ROOT, "disc", "Grandia Xtreme.ISO")
DEFAULT_RAR = os.path.join(ROOT, "Grandia Xtreme (USA).rar")
BOOT_FILES = ["SLUS_204.17", "FIELD.BIN", "BATTLE.BIN", "VIEWER.BIN"]


# ------------------------------------------------------------------ ISO9660
class Record:
    def __init__(self, pos, raw, path):
        self.pos = pos  # byte offset of the record in the image
        self.extent = struct.unpack_from("<I", raw, 2)[0]
        self.size = struct.unpack_from("<I", raw, 10)[0]
        self.flags = raw[25]
        self.path = path

    @property
    def is_dir(self):
        return bool(self.flags & 2)


def volume_descriptors(f):
    """(sector, type) of the primary/supplementary volume descriptors."""
    out = []
    s = 16
    while True:
        f.seek(s * SECTOR)
        d = f.read(SECTOR)
        if d[1:6] != b"CD001":
            raise SystemExit(f"sector {s}: not an ISO9660 volume descriptor")
        if d[0] == 255:
            return out
        if d[0] in (1, 2):
            out.append((s, d[0]))
        s += 1


def walk(f, vd_sector):
    """Every directory record under the root of a volume descriptor, by path."""
    f.seek(vd_sector * SECTOR)
    vd = f.read(SECTOR)
    if struct.unpack_from("<H", vd, 128)[0] != SECTOR:
        raise SystemExit("logical block size isn't 2048")
    root = Record(vd_sector * SECTOR + 156, vd[156:156 + 34], "")
    out = []
    todo = [root]
    seen = set()
    while todo:
        d = todo.pop()
        if d.extent in seen:
            continue
        seen.add(d.extent)
        f.seek(d.extent * SECTOR)
        data = f.read(d.size)
        o = 0
        while o < len(data):
            n = data[o]
            if n == 0:  # records don't cross sectors: skip to the next one
                o = (o // SECTOR + 1) * SECTOR
                continue
            raw = data[o:o + n]
            name_len = raw[32]
            name = raw[33:33 + name_len]
            if name not in (b"\0", b"\1"):
                r = Record(d.extent * SECTOR + o, raw, (d.path + "/" if d.path else "") +
                           name.decode("latin1").split(";")[0])
                out.append(r)
                if r.is_dir:
                    todo.append(r)
            o += n
    return out


def files(path):
    """path -> Record for every file of every volume descriptor (the first wins)."""
    out = {}
    allrecs = []
    with open(path, "rb") as f:
        for s, t in volume_descriptors(f):
            for r in walk(f, s):
                allrecs.append(r)
                if not r.is_dir:
                    out.setdefault(r.path.upper(), r)
    return out, allrecs


def read_file(f, r):
    f.seek(r.extent * SECTOR)
    return f.read(r.size)


# ------------------------------------------------------------------ patch
def rar_crc(rar):
    """CRC-32 and size of the (only) file in a RAR 4 archive, from its header."""
    with open(rar, "rb") as f:
        if f.read(7) != b"Rar!\x1a\x07\x00":
            raise SystemExit(f"{rar}: not a RAR 4 archive")
        pos = 7
        while True:
            f.seek(pos)
            h = f.read(32)
            if len(h) < 7:
                raise SystemExit(f"{rar}: no file header")
            crc, typ, flags, size = struct.unpack_from("<HBHH", h)
            if typ == 0x74:
                pack, unp, host, fcrc = struct.unpack_from("<IIBI", h, 7)
                return fcrc, unp
            add = struct.unpack_from("<I", h, 7)[0] if flags & 0x8000 else 0
            pos += size + add


def crc32_file(path):
    c = 0
    with open(path, "rb") as f:
        while True:
            b = f.read(1 << 24)
            if not b:
                return c
            c = zlib.crc32(b, c)


def clone(src, dst):
    if os.path.exists(dst):
        os.remove(dst)
    if sys.platform == "darwin":
        if subprocess.run(["cp", "-c", src, dst], stderr=subprocess.DEVNULL).returncode == 0:
            return "clone"
    shutil.copyfile(src, dst)
    return "copy"


GRX_INI = {
    # The game reads GRX.INI from the disc's root at boot (m:40D860.c,
    # SysIniLoad): 10-character keys (sysIniKeyTable), the value after them
    # (tabs ignored), CR LF line ends, "[end]". Without the file it boots
    # with program 1, frame skip, motion smoothing 10, collision and MDT data
    # on, vibration off; with it, only what the file says, so these repeat
    # those defaults.
    "debug": ["program no\t1", "debug mode\ty"],
    "viewer": ["program no\t0", "debug mode\ty"],
}
GRX_INI_DEFAULTS = ["frame mode\t0", "frame skip\ty", "motion smt\t10", "collision0\ty",
                    "usemdtdata\ty", "pad_vibrat\tn"]


def grx_ini(kind):
    lines = [f"; GRX.INI written by tools/iso_patch.py --grx-ini {kind}"] + GRX_INI[kind] + GRX_INI_DEFAULTS
    return ("\r\n".join(lines + ["[end]"]) + "\r\n").encode("ascii")


def iso_name(name):
    return name.upper().encode("ascii") + b";1"


def add_records(f, adds, lba_of):
    """Add a file record for each of `adds` ({name: size}) to the root directory
    of every volume descriptor, sorted by name; the root's size grows in its
    PVD record, its "." record and every subdirectory's ".." record.
    Returns the sectors written."""
    touched = set()
    for vd, t in volume_descriptors(f):
        f.seek(vd * SECTOR)
        pvd = f.read(SECTOR)
        root = Record(vd * SECTOR + 156, pvd[156:190], "")
        f.seek(root.extent * SECTOR)
        data = bytearray(f.read(root.size))
        recs = []
        o = 0
        while o < len(data):
            n = data[o]
            if n == 0:
                raise SystemExit("the root directory spans sectors; adding files there isn't supported")
            recs.append(bytes(data[o:o + n]))
            o += n
        template = next(r for r in recs[2:] if not r[25] & 2)
        for name, size in adds.items():
            nm = iso_name(name)
            if any(r[33:33 + r[32]] == nm for r in recs):
                raise SystemExit(f"{name} is already on the disc: replace it instead")
            rec = bytearray(template[:33]) + nm
            if len(rec) % 2:
                rec += b"\0"
            rec[0] = len(rec)
            lba = lba_of[name]
            rec[2:10] = struct.pack("<I", lba) + struct.pack(">I", lba)
            rec[10:18] = struct.pack("<I", size) + struct.pack(">I", size)
            rec[25] = 0
            rec[32] = len(nm)
            # this disc lists the directories first, then the files by name
            i = len(recs)
            for k in range(2, len(recs)):
                if not recs[k][25] & 2 and recs[k][33:33 + recs[k][32]] > nm:
                    i = k
                    break
            recs.insert(i, bytes(rec))
        new = b"".join(recs)
        if len(new) > SECTOR:
            raise SystemExit("no room in the root directory's sector")
        size = len(new)
        f.seek(root.extent * SECTOR)
        f.write(new)
        touched.add(root.extent)
        # the root's size: its PVD record, its "." record, the subdirectories' ".."
        for pos in [vd * SECTOR + 156, root.extent * SECTOR]:
            f.seek(pos + 10)
            f.write(struct.pack("<I", size) + struct.pack(">I", size))
        touched.add(vd)
        o = 0
        while o < size:
            n = new[o]
            r = Record(0, new[o:o + n], "")
            if r.is_dir and not (new[o + 32] == 1 and new[o + 33] in (0, 1)):
                f.seek(r.extent * SECTOR)
                sub = f.read(SECTOR)
                dotdot = sub[0]  # "." comes first, ".." second
                f.seek(r.extent * SECTOR + dotdot + 10)
                f.write(struct.pack("<I", size) + struct.pack(">I", size))
                touched.add(r.extent)
            o += n
    return touched


def patch(src, out, repl, adds=None):
    """repl: {ISO path: bytes} files to replace; adds: {name: bytes} new files
    in the root directory. Returns (the sectors whose contents may differ
    from the source, how the copy was made)."""
    adds = adds or {}
    recs, allrecs = files(src)
    targets = {}
    for name in repl:
        key = name.upper()
        if key not in recs:
            raise SystemExit(f"{name}: not on the disc")
        targets[key] = [r for r in allrecs if r.path.upper() == key]
        if any(r.flags & 0x80 for r in targets[key]):
            raise SystemExit(f"{name}: multi-extent files aren't supported")
    how = clone(src, out)
    touched = set()
    with open(out, "r+b") as f:
        f.seek(0, 2)
        end = f.tell()
        if end % SECTOR:
            raise SystemExit(f"{src}: size isn't a multiple of {SECTOR}")
        lba = end // SECTOR
        lba_of = {}
        for name, data in list(repl.items()) + list(adds.items()):
            f.seek(lba * SECTOR)
            f.write(data)
            f.write(bytes(-len(data) % SECTOR))
            lba_of[name] = lba
            for r in targets.get(name.upper(), []):
                f.seek(r.pos + 2)
                f.write(struct.pack("<I", lba) + struct.pack(">I", lba))
                f.write(struct.pack("<I", len(data)) + struct.pack(">I", len(data)))
                touched.add(r.pos // SECTOR)
            lba += (len(data) + SECTOR - 1) // SECTOR
        if adds:
            touched |= add_records(f, {n: len(d) for n, d in adds.items()}, lba_of)
        for s, t in volume_descriptors(f):
            f.seek(s * SECTOR + 80)
            f.write(struct.pack("<I", lba) + struct.pack(">I", lba))
            touched.add(s)
    return touched, how


def verify(src, out, repl, adds, touched):
    """Read the patched image back; returns the report lines (raises on a mismatch)."""
    lines = []
    srecs, _ = files(src)
    orecs, _ = files(out)
    new = {k.upper() for k in adds}
    if set(orecs) != set(srecs) | new:
        raise SystemExit("the patched image has a different file list")
    with open(src, "rb") as fs, open(out, "rb") as fo:
        for key, r in sorted(orecs.items()):
            got = read_file(fo, r)
            if key in {k.upper() for k in repl}:
                want = next(v for k, v in repl.items() if k.upper() == key)
                if got != want:
                    raise SystemExit(f"{key}: reads back differently from its input")
                lines.append(f"replaced  {key:24} sector {srecs[key].extent} -> {r.extent}, "
                             f"{srecs[key].size} -> {r.size} bytes: reads back identical to its input")
            elif key in new:
                want = next(v for k, v in adds.items() if k.upper() == key)
                if got != want:
                    raise SystemExit(f"{key}: reads back differently from its input")
                lines.append(f"added     {key:24} sector {r.extent}, {r.size} bytes: reads back identical")
            else:
                s = srecs[key]
                if (s.extent, s.size) != (r.extent, r.size) or got != read_file(fs, s):
                    raise SystemExit(f"{key}: changed although it wasn't replaced")
        lines.append(f"unchanged {len(orecs) - len(repl) - len(new)} other files: same sectors, same bytes")
        # which sectors of the source's length differ
        size = os.path.getsize(src)
        diff = []
        chunk = 1 << 26
        fs.seek(0)
        fo.seek(0)
        pos = 0
        while pos < size:
            a = fs.read(chunk)
            b = fo.read(len(a))
            if a != b:
                for i in range(0, len(a), SECTOR):
                    if a[i:i + SECTOR] != b[i:i + SECTOR]:
                        diff.append((pos + i) // SECTOR)
            pos += len(a)
    unexpected = [s for s in diff if s not in touched]
    lines.append(f"sectors that differ from the source within its {size // SECTOR} sectors: "
                 f"{', '.join(map(str, diff)) or 'none'} (volume descriptors and directory sectors "
                 f"rewritten: {', '.join(map(str, sorted(touched)))})")
    if unexpected:
        raise SystemExit(f"unexpected differing sectors: {unexpected[:20]}")
    extra = os.path.getsize(out) - size
    lines.append(f"appended {extra} bytes ({extra // SECTOR} sectors) after the source's end")
    return lines


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="*", help="ISO_NAME=path")
    ap.add_argument("--dir", help="take SLUS_204.17, FIELD.BIN, BATTLE.BIN and VIEWER.BIN from here")
    ap.add_argument("--add", nargs="*", default=[], metavar="NAME=path",
                    help="add a new file to the root directory")
    ap.add_argument("--grx-ini", choices=sorted(GRX_INI),
                    help="add a GRX.INI: debug (normal boot, debug mode on) or viewer (boots VIEWER)")
    ap.add_argument("--src", default=DEFAULT_SRC)
    ap.add_argument("--out", required=True)
    ap.add_argument("--rar", default=DEFAULT_RAR)
    ap.add_argument("--no-rar-check", action="store_true")
    args = ap.parse_args()
    repl = {}
    if args.dir:
        for n in BOOT_FILES:
            repl[n] = open(os.path.join(args.dir, n), "rb").read()
    for item in args.files:
        n, _, p = item.partition("=")
        repl[n] = open(p, "rb").read()
    adds = {}
    for item in args.add:
        n, _, p = item.partition("=")
        adds[n] = open(p, "rb").read()
    if args.grx_ini:
        adds["GRX.INI"] = grx_ini(args.grx_ini)
    if not repl and not adds:
        raise SystemExit("nothing to replace or add")
    out = os.path.abspath(args.out)
    if out.startswith(ROOT + os.sep) and not out.startswith(os.path.join(ROOT, "build") + os.sep):
        raise SystemExit("write images under build/ (gitignored) or outside the repository")
    if not args.no_rar_check and os.path.exists(args.rar):
        crc, size = rar_crc(args.rar)
        if os.path.getsize(args.src) != size or crc32_file(args.src) != crc:
            raise SystemExit(f"{args.src} isn't the file in {args.rar} (CRC-32 {crc:08X}, {size} bytes)")
        print(f"source {args.src}: CRC-32 {crc:08X} and size match {os.path.basename(args.rar)}")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    touched, how = patch(args.src, out, repl, adds)
    print(f"wrote {args.out} ({how} of the source, {len(repl)} files replaced, {len(adds)} added)")
    lines = verify(args.src, out, repl, adds, touched)
    for line in lines:
        print(line)
    what = [f"{k} ({len(v)} bytes)" for k, v in repl.items()] + [f"new {k} ({len(v)} bytes)" for k, v in adds.items()]
    with open(out + ".txt", "w") as f:
        f.write(f"{os.path.basename(out)}: {args.src} with {', '.join(what)}"
                f"{' from ' + args.dir if args.dir else ''}"
                f"{' and GRX.INI ' + args.grx_ini if args.grx_ini else ''}\n" + "\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
