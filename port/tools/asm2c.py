"""Translate the game's inline asm into C for the port build.

The matching C keeps the original instructions in inline asm (quadword
copies, sqrt.s/min.s/max.s, VU0 macro mode, multu, sync). A port build
can't assemble R5900 code, so each asm statement is rewritten into calls on
the port's emulation of those instructions (port/include/grx_ee.h): the VU0
registers, ACC and the GPRs named in templates are global state there, so
asm statements that hand values to each other through registers still work.

The rewrite keeps every newline of the original statement (after it, before
the `;`), so line numbers stay right and statements inside macros stay
inside them. Anything the translator doesn't know is an error, never
silently dropped.
"""

import re


class AsmError(Exception):
    pass


# --- scanning C ----------------------------------------------------------------

def _skip_string(s, i):
    """s[i] is a quote; return the index after the closing quote."""
    q = s[i]
    i += 1
    while s[i] != q:
        i += 2 if s[i] == "\\" else 1
    return i + 1


def find_asm(src):
    """Yield (start, end) of every `__asm__ [volatile] (...)` in src, ignoring
    comments and strings."""
    i, n = 0, len(src)
    kw = re.compile(r"\b(?:__asm__|__asm|asm)\b(?:\s+(?:volatile|__volatile__))?\s*\(")
    while i < n:
        c = src[i]
        if src.startswith("//", i):
            i = src.find("\n", i)
            i = n if i < 0 else i
        elif src.startswith("/*", i):
            i = src.find("*/", i) + 2
        elif c in "\"'":
            i = _skip_string(src, i)
        else:
            m = kw.match(src, i)
            if m and (i == 0 or not (src[i - 1].isalnum() or src[i - 1] == "_")):
                j, depth = m.end(), 1
                while depth:
                    if src[j] in "\"'":
                        j = _skip_string(src, j)
                        continue
                    depth += {"(": 1, ")": -1}.get(src[j], 0)
                    j += 1
                yield i, j, src[m.end():j - 1]
                i = j
            else:
                i += 1


def _split_top(s, sep):
    """Split s at sep outside strings and brackets."""
    parts, depth, cur, i = [], 0, [], 0
    while i < len(s):
        c = s[i]
        if c in "\"'":
            j = _skip_string(s, i)
            cur.append(s[i:j])
            i = j
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
        i += 1
    parts.append("".join(cur))
    return parts


_ESC = {"n": "\n", "t": "\t", '"': '"', "\\": "\\", "'": "'"}


def _template(s):
    """The template as a list of pieces: str for literal text, ('id', name)
    for a macro parameter pasted in between literals."""
    s = s.replace("\\\n", " ")
    out, i = [], 0
    while i < len(s):
        c = s[i]
        if c == '"':
            j = _skip_string(s, i)
            lit = re.sub(r"\\(.)", lambda m: _ESC.get(m.group(1), m.group(1)), s[i + 1:j - 1])
            out.append(lit)
            i = j
        elif c.isspace():
            i += 1
        else:
            m = re.match(r"[A-Za-z_]\w*", s[i:])
            if not m:
                raise AsmError(f"unexpected template text {s[i:]!r}")
            out.append(("id", m.group(0)))
            i += m.end()
    return out


def _operands(s):
    ops = []
    for part in _split_top(s, ","):
        part = part.replace("\\\n", " ").strip()
        if not part:
            continue
        m = re.match(r'(?:\[(\w+)\]\s*)?"([^"]*)"\s*\((.*)\)$', part, re.S)
        if not m:
            raise AsmError(f"can't parse operand {part!r}")
        ops.append({"name": m.group(1), "cons": m.group(2), "expr": m.group(3).strip()})
    return ops


def _lvalue(expr):
    """Outputs like "=l"((u32)(w0)) are casts gcc 2.9 accepted as lvalues."""
    m = re.match(r"\(\s*[A-Za-z_][\w\s]*\*?\s*\)\s*(\(.*\))$", expr, re.S)
    return m.group(1) if m else f"({expr})"


# --- translating instructions -----------------------------------------------------

GPR_NAMES = {n: i for i, n in enumerate(
    "zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 "
    "s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra".split())}

VU_ARITH = {"add": "GRX_VADD", "sub": "GRX_VSUB", "mul": "GRX_VMUL",
            "madd": "GRX_VMADD", "msub": "GRX_VMSUB", "max": "GRX_VMAX",
            "mini": "GRX_VMINI"}
FIELDS = "xyzw"


class Translator:
    def __init__(self, ops):
        self.ops = ops

    def op(self, ref):
        # %zN is the operand, or $zero when it is the constant 0
        m = re.fullmatch(r"%z?(\d+)|%z?\[(\w+)\]", ref)
        if not m:
            return None
        if m.group(1) is not None:
            return self.ops[int(m.group(1))]
        for o in self.ops:
            if o["name"] == m.group(2):
                return o
        raise AsmError(f"no operand {ref}")

    def value(self, ref):
        o = self.op(ref)
        if o is None:
            raise AsmError(f"expected an operand, got {ref!r}")
        return _lvalue(o["expr"]) if "=" in o["cons"] or "+" in o["cons"] else f"({o['expr']})"

    def gpr(self, ref):
        """A GPR as a pointer to its 16 bytes: a named register is the port's
        register file, an operand is its own variable."""
        m = re.fullmatch(r"\$(\d+|[a-z]\w*)", ref)
        if m:
            r = m.group(1)
            return f"&grx_gpr[{int(r) if r.isdigit() else GPR_NAMES[r]}]"
        return None

    def vf(self, ref):
        m = re.fullmatch(r"\$vf(\d+)([xyzw]?)", ref)
        if not m:
            raise AsmError(f"expected a VF register, got {ref!r}")
        return int(m.group(1)), (FIELDS.index(m.group(2)) if m.group(2) else -1)

    def vreg(self, ref):
        if ref == "ACC":
            return "GRX_ACC", -1
        n, bc = self.vf(ref)
        return str(n), bc

    @staticmethod
    def mem(arg):
        m = re.fullmatch(r"(-?(?:0x[0-9A-Fa-f]+|\d+))?\((.+)\)", arg)
        if not m:
            raise AsmError(f"expected off(base), got {arg!r}")
        return m.group(1) or "0", m.group(2)

    def addr(self, arg):
        off, base = self.mem(arg)
        if base.startswith("$"):
            raise AsmError(f"register base {base} not supported")
        return f"GRX_ADDR({self.value(base)}, {off})"

    def insn(self, mn, args):
        dyn = isinstance(mn, tuple)
        if dyn:
            # A mnemonic pasted in from a macro parameter: decode at run time
            if len(args) != 3:
                raise AsmError(f"dynamic mnemonic with {len(args)} operands")
            d, s, t = (self.vreg(a) for a in args)
            return f"grx_vu0_named({mn[1]}, {d[0]}, {s[0]}, {t[0]}, {t[1]});"
        base, _, dest = mn.partition(".")
        mask = sum(1 << FIELDS.index(c) for c in dest) if dest and set(dest) <= set(FIELDS) else 0

        if base in ("sync",):
            return "grx_sync();"
        if base in ("lq", "sq"):
            g = self.gpr(args[0])
            if g is None:
                v = self.value(args[0])
                g = f"(void *)&{v}"
            fn = "grx_lq" if base == "lq" else "grx_sq"
            return f"{fn}({g}, {self.addr(args[1])});"
        if base in ("lqc2", "sqc2"):
            n, bc = self.vf(args[0])
            return f"grx_{base}({n}, {self.addr(args[1])});"
        if base == "qmtc2":
            n, _ = self.vf(args[1])
            g = self.gpr(args[0])
            return (f"grx_qmtc2_q({n}, {g});" if g else f"GRX_QMTC2({n}, {self.value(args[0])});")
        if base == "qmfc2":
            n, _ = self.vf(args[1])
            g = self.gpr(args[0])
            return (f"grx_qmfc2_q({g}, {n});" if g else f"GRX_QMFC2({self.value(args[0])}, {n});")
        if base == "cfc2":
            m = re.fullmatch(r"\$vi(\d+)", args[1])
            return f"{self.value(args[0])} = grx_cfc2({m.group(1)});"
        if base == "ctc2":
            m = re.fullmatch(r"\$vi(\d+)", args[1])
            return f"grx_ctc2({m.group(1)}, {self.value(args[0])});"
        if base == "sqrt" and dest == "s":
            return f"{self.value(args[0])} = grx_sqrt_s({self.value(args[1])});"
        if base in ("min", "max") and dest == "s":
            return f"{self.value(args[0])} = grx_{base}_s({self.value(args[1])}, {self.value(args[2])});"
        if base == "trunc" and dest == "w.s":
            return f"GRX_TRUNC_W_S({self.value(args[0])}, {self.value(args[1])});"
        if base == "mfc1":
            return f"GRX_MFC1({self.value(args[0])}, {self.value(args[1])});"
        if base == "mtc1":
            return f"GRX_MTC1({self.value(args[1])}, {self.value(args[0])});"
        if base == "multu":
            lo = [o for o in self.ops if "l" in o["cons"].lstrip("=+&")]
            hi = [o for o in self.ops if "h" in o["cons"].lstrip("=+&")]
            st = f"unsigned long long _p = (unsigned long long)(unsigned int){self.value(args[0])} * (unsigned int){self.value(args[1])};"
            if lo:
                st += f" {_lvalue(lo[0]['expr'])} = (unsigned int)_p;"
            if hi:
                st += f" {_lvalue(hi[0]['expr'])} = (unsigned int)(_p >> 32);"
            return "{ " + st + " }"

        # VU0 microprograms, rewritten as C (port/src/vu0_micro.c): the
        # address as written is the byte address
        if base == "vcallms":
            return f"grx_vu0_callms({args[0]});"
        if base == "vcallmsr":
            return "grx_vu0_callmsr();"

        if base.startswith("v"):
            return self.vu(base[1:], mask, args)
        raise AsmError(f"unknown instruction {mn!r}")

    def vu(self, op, mask, args):
        if op == "opmula":
            _, s, t = (self.vreg(a) for a in args)
            return f"grx_vu0_opmula({s[0]}, {t[0]});"
        if op == "opmsub":
            d, s, t = (self.vreg(a) for a in args)
            return f"grx_vu0_opmsub({d[0]}, {s[0]}, {t[0]});"
        if op in ("mr32", "move", "abs"):
            d, s = (self.vreg(a) for a in args)
            return f"grx_vu0_{op}({mask}, {d[0]}, {s[0]});"
        m = re.fullmatch(r"(itof|ftoi)(0|4|12|15)", op)
        if m:
            d, s = (self.vreg(a) for a in args)
            return f"grx_vu0_{m.group(1)}({mask}, {d[0]}, {s[0]}, {m.group(2)});"
        m = re.fullmatch(r"(add|sub|mul|madd|msub|max|mini)(a?)([xyzwiq]?)", op)
        if m:
            arith, to_acc, bcf = m.groups()
            d, s, t = (self.vreg(a) for a in args)
            if to_acc and d[0] != "GRX_ACC":
                raise AsmError(f"v{op} writes ACC, got {args[0]}")
            if bcf in ("i", "q"):
                raise AsmError(f"v{op}: I/Q registers not supported")
            bc = FIELDS.index(bcf) if bcf else -1
            if bc >= 0 and t[1] != bc:
                raise AsmError(f"v{op}: broadcast field doesn't match {args[2]}")
            return f"grx_vu0_arith({VU_ARITH[arith]}, {mask}, {d[0]}, {s[0]}, {t[0]}, {bc});"
        raise AsmError(f"unknown VU0 instruction v{op}")


def translate_body(body):
    """Body of one asm statement (inside its parens) -> C statement."""
    parts = _split_top(body, ":")
    tmpl = _template(parts[0])
    outs = _operands(parts[1]) if len(parts) > 1 else []
    ins = _operands(parts[2]) if len(parts) > 2 else []
    tr = Translator(outs + ins)

    # Instructions: split the literal text at newlines and ';'; a pasted
    # parameter stands for a mnemonic
    insns, cur = [], []
    for piece in tmpl:
        if isinstance(piece, tuple):
            cur.append(piece)
            continue
        lines = re.split(r"[\n;]", piece)
        for k, line in enumerate(lines):
            if k:
                insns.append(cur)
                cur = []
            if line.strip():
                cur.append(line)
    insns.append(cur)

    code = []
    for ins in insns:
        if not ins:
            continue
        if isinstance(ins[0], tuple):
            mn, rest = ins[0], "".join(p for p in ins[1:] if isinstance(p, str))
        else:
            text = "".join(p if isinstance(p, str) else "" for p in ins).strip()
            if any(isinstance(p, tuple) for p in ins):
                raise AsmError(f"parameter inside an instruction: {ins!r}")
            if not text or text.startswith("#"):
                continue
            mn, _, rest = text.partition(" ")
            if text.startswith("."):
                continue  # .set directives
        args = [a.strip() for a in _split_top(rest.strip(), ",") if a.strip()]
        code.append(tr.insn(mn, args))
    if not code:
        return "((void)0)"
    return "do { " + " ".join(code) + " } while (0)"


def translate(src):
    """Rewrite every asm statement in a C source; returns (new source, count)."""
    out, last, count = [], 0, 0
    for start, end, body in find_asm(src):
        try:
            c = translate_body(body)
        except (AsmError, IndexError, KeyError, ValueError) as e:
            line = src.count("\n", 0, start) + 1
            raise AsmError(f"line {line}: {e}") from None
        breaks = re.findall(r"\\?\n", src[start:end])
        out.append(src[last:start])
        out.append(c + "".join(breaks))
        last = end
        count += 1
    out.append(src[last:])
    return "".join(out), count
