"""Talk to the freeze tool in a running game (port/docs/debug.md), or read a
dump it wrote.

    python port/tools/grxdbg.py status
    python port/tools/grxdbg.py freeze | resume | step [N]
    python port/tools/grxdbg.py read camField 0x180 --as f32
    python port/tools/grxdbg.py read sysDemo+0x50 16
    python port/tools/grxdbg.py read ee:0x200000 64     # a PS2 main-RAM address
    python port/tools/grxdbg.py write sysDemo+0x50 --u32 0
    python port/tools/grxdbg.py sym camField            # names matching, with addresses
    python port/tools/grxdbg.py threads | log [BYTES] | inputs [FIELDS]
    python port/tools/grxdbg.py dump [DIR]
    python port/tools/grxdbg.py quit                    # end the game
    python port/tools/grxdbg.py --dump build/freeze/<dir> read camField 0x180 --as f32

Names come from the link map (build/port/grandia.map, or --map): the
game's globals and functions by their C names. An address is NAME,
NAME+OFFSET, a number, or ee:PS2ADDRESS. The game must run with the server
started: it starts at the first freeze (F9), or at boot with
GRX_DEBUG_PORT set.
"""
import argparse
import bisect
import json
import os
import re
import socket
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
FORMATS = {"u8": "B", "s8": "b", "u16": "H", "s16": "h", "u32": "I", "s32": "i", "f32": "f", "u64": "Q"}


class Symbols:
    """The link map's symbols: name -> address (relative to the image base)"""

    def __init__(self, path):
        self.by_name = {}
        line_re = re.compile(r"^([0-9a-f]{8}) ([0-9a-f]{8}) +\d+ +(\S+)$")
        if not os.path.exists(path):
            return
        with open(path, errors="replace") as f:
            for line in f:
                m = line_re.match(line.rstrip("\n"))
                if not m:
                    continue
                name = m.group(3)
                if name.startswith((".", "__imp_")) or ":" in name or "(" in name:
                    continue
                if name.startswith("_"):
                    name = name[1:]  # i686 C names
                self.by_name.setdefault(name, int(m.group(1), 16))
        self.addrs = sorted(set(self.by_name.values()))
        self.names_at = {}
        for n, a in self.by_name.items():
            self.names_at.setdefault(a, n)

    def size(self, rva):
        i = bisect.bisect_right(self.addrs, rva)
        return self.addrs[i] - rva if i < len(self.addrs) else None

    def near(self, rva):
        i = bisect.bisect_right(self.addrs, rva) - 1
        if i < 0:
            return None
        a = self.addrs[i]
        return self.names_at[a], rva - a


class Live:
    def __init__(self, port):
        self.s = socket.create_connection(("127.0.0.1", port), timeout=600)
        self.f = self.s.makefile("rb")

    def ask(self, line):
        self.s.sendall(line.encode() + b"\n")
        ans = json.loads(self.f.readline())
        if not ans.get("ok"):
            sys.exit("game: " + ans.get("error", "failed"))
        return ans

    def image_base(self):
        return self.ask("status")["image_base"]

    def read(self, addr, n):
        return bytes.fromhex(self.ask(f"read {addr:#x} {n}")["hex"])

    def ee(self, ps2):
        return self.ask(f"ee {ps2:#x}")["host"]


class Dump:
    """memory.bin, indexed by memory.txt"""

    def __init__(self, d):
        self.dir = d
        self.ranges = []
        with open(os.path.join(d, "memory.txt")) as f:
            for line in f:
                if line.startswith("#"):
                    m = re.search(r"image base (0x[0-9a-f]+)", line)
                    self.base = int(m.group(1), 16)
                    continue
                name, a, n, off = line.split()
                self.ranges.append((name, int(a, 16), int(n, 16), int(off, 16)))
        self.mem = open(os.path.join(d, "memory.bin"), "rb")

    def ask(self, line):
        cmd = line.split()[0]
        if cmd in ("log", "threads"):
            with open(os.path.join(self.dir, "log.txt" if cmd == "log" else "threads.txt")) as f:
                return {"text": f.read()}
        if cmd == "inputs":
            with open(os.path.join(self.dir, "inputs.txt")) as f:
                return {"lines": f.read().splitlines()}
        if cmd == "status":
            with open(os.path.join(self.dir, "state.txt")) as f:
                return dict(l.split(" ", 1) for l in f.read().splitlines())
        sys.exit(f"{cmd}: not in a dump")

    def image_base(self):
        return self.base

    def read(self, addr, n):
        for name, a, size, off in self.ranges:
            if a <= addr and addr + n <= a + size:
                self.mem.seek(off + addr - a)
                return self.mem.read(n)
        sys.exit(f"{addr:#x}+{n}: not in the dump ({', '.join(r[0] for r in self.ranges)})")

    def ee(self, ps2):
        sys.exit("ee: addresses need the live game")


def resolve(expr, syms, target):
    """NAME[+OFF], NUMBER or ee:PS2 -> (host address, size if known)"""
    if expr.startswith("ee:"):
        return target.ee(int(expr[3:], 0)), None
    m = re.match(r"^([A-Za-z_][\w$.@]*)(?:([+-])(\w+))?$", expr)
    if m and m.group(1) in syms.by_name:
        rva = syms.by_name[m.group(1)]
        off = int(m.group(3), 0) * (-1 if m.group(2) == "-" else 1) if m.group(3) else 0
        base = target.image_base()
        return (rva if rva >= base else base + rva) + off, syms.size(rva) if not off else None
    try:
        return int(expr, 0), None
    except ValueError:
        sys.exit(f"{expr}: no such name in the map")


def show(data, addr, fmt, syms, base):
    if fmt in (None, "hex"):
        for i in range(0, len(data), 16):
            chunk = data[i:i + 16]
            hexs = " ".join(f"{b:02x}" for b in chunk)
            text = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
            print(f"{addr + i:08x}  {hexs:<47}  {text}")
    elif fmt == "str":
        print(data.split(b"\0")[0].decode("latin-1"))
    elif fmt == "ptr":
        for i in range(0, len(data) - 3, 4):
            v = struct.unpack_from("<I", data, i)[0]
            near = syms.near(v - base) if v >= base else None
            print(f"{addr + i:08x}  {v:08x}" + (f"  {near[0]}+{near[1]:#x}" if near else ""))
    else:
        c = FORMATS[fmt]
        w = struct.calcsize(c)
        vals = struct.unpack_from("<" + c * (len(data) // w), data)
        per = 4 if w >= 4 else 8
        for i in range(0, len(vals), per):
            row = vals[i:i + per]
            s = "  ".join(f"{v:12.4f}" if fmt == "f32" else f"{v:10}" for v in row)
            print(f"{addr + i * w:08x} +{i * w:#06x}  {s}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", type=int, default=int(os.environ.get("GRX_DEBUG_PORT", "7790")))
    ap.add_argument("--map", default=os.path.join(ROOT, "build", "port", "grandia.map"))
    ap.add_argument("--dump", help="read a dump directory instead of the live game")
    ap.add_argument("--as", dest="fmt", choices=["hex", "str", "ptr"] + list(FORMATS))
    ap.add_argument("--u32", type=lambda s: int(s, 0))
    ap.add_argument("--f32", type=float)
    ap.add_argument("cmd")
    ap.add_argument("args", nargs="*")
    a = ap.parse_args()
    syms = Symbols(a.map)
    target = Dump(a.dump) if a.dump else Live(a.port)

    if a.cmd == "sym":
        pat = re.compile(a.args[0] if a.args else ".", re.I)
        base = target.image_base()
        for n, rva in sorted(syms.by_name.items(), key=lambda kv: kv[1]):
            if pat.search(n):
                size = syms.size(rva)
                print(f"{(rva if rva >= base else base + rva):08x} {size or 0:8}  {n}")
    elif a.cmd == "read":
        addr, size = resolve(a.args[0], syms, target)
        n = int(a.args[1], 0) if len(a.args) > 1 else min(size if size and size >= 16 else 64, 4096)
        show(target.read(addr, n), addr, a.fmt, syms, target.image_base())
    elif a.cmd == "write":
        addr, _ = resolve(a.args[0], syms, target)
        if a.u32 is not None:
            data = struct.pack("<I", a.u32 & 0xFFFFFFFF)
        elif a.f32 is not None:
            data = struct.pack("<f", a.f32)
        else:
            data = bytes.fromhex(a.args[1])
        print(target.ask(f"write {addr:#x} {data.hex()}"))
    elif a.cmd == "addr":
        addr, size = resolve(a.args[0], syms, target)
        print(f"{addr:#010x}" + (f" ({size} bytes to the next symbol)" if size else ""))
    elif a.cmd in ("log", "threads"):
        print(target.ask(" ".join([a.cmd] + a.args))["text"], end="")
    elif a.cmd == "quit":
        target.s.sendall(b"quit\n")
    elif a.cmd == "inputs":
        print("\n".join(target.ask(" ".join([a.cmd] + a.args))["lines"]))
    else:
        print(json.dumps(target.ask(" ".join([a.cmd] + a.args)), indent=1))


if __name__ == "__main__":
    main()
