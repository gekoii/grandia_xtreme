#!/usr/bin/env python3
"""32-bit pointer fields for the x86-64 build (port/docs/win64.md).

On the PS2 (and i686) a pointer is 4 bytes. On x86-64 every pointer field
of the game's structs stays 4 bytes too: an EE address, which works
because the game's memory is below 2 GB. Then every struct keeps the
PS2's layout, so the disc's data, the MIPS programs, the hardware's
formats and the decompilation's several views of one memory all agree.

This rewrites the game's C to do it, from clang's AST (-ast-dump=json) of
each generated source as build.py makes it:

  declarations  a pointer field of a game struct (in include/, src/ or
                port/game/), or an array of them, becomes `GRX_EE32 name`
                (unsigned int; `name[N]` for an array);
  reads         an rvalue use of one becomes GRX_EE_AS(P, use): a P again,
                so arithmetic, calls through it and comparisons are the
                pointer's;
  writes        `use = value` stores GRX_EE_FROM(value), the low 32 bits;
  += -= ++ --   go through the pointer type (GRX_EE_ADD, GRX_EE_POSTADD),
                so they step by elements, not bytes;
  initialisers  a static object's address constant in such a field can't
                be a constant 32-bit value: it is 0 in the initialiser, and
                a constructor at start-up stores it (dropping the object's
                const, so it can);
  sizeof        stays: 4, as on the PS2.

Anything else on such a field (its address taken, an array of them used
whole, a use the rewrite can't place in a macro) stops with the source
and line, for a fixup in port/tools/fixups/ to settle.

Port-only types (port/include/, port/src/) are left alone: they hold host
pointers. Only the x86-64 build uses this; i686 compiles build.py's copies
as they are.

The helpers (GRX_EE32, GRX_EE_AS, ...) are in port/include/grx_ee64.h.
"""

import hashlib
import json
import os
import re
import subprocess

VERSION = 30  # bump when the rewrite changes: the per-source cache is keyed by it


class Fail(Exception):
    pass


# ---------------------------------------------------------------------------
# Locations

class Loc:
    """A location in clang's JSON: the file (as clang names it), byte offset,
    token length; for a macro, the expansion's offset too"""
    __slots__ = ("file", "off", "tok", "macro", "exp_off", "exp_file", "arg")

    def __init__(self, file, off, tok, macro=False, exp_off=None, exp_file=None, arg=False):
        self.file, self.off, self.tok, self.macro = file, off, tok, macro
        self.exp_off, self.exp_file, self.arg = exp_off, exp_file, arg


class Decoder:
    """clang's JSON prints a location's file only when it changes from the
    one printed before: decode in print order, keeping the last file"""

    def __init__(self):
        self.file = None

    def bare(self, o):
        if "file" in o:
            self.file = o["file"]
        return self.file, o.get("offset"), o.get("tokLen", 0)

    def loc(self, o):
        if not o:
            return None
        if "spellingLoc" in o:
            sf, so, st = self.bare(o["spellingLoc"])
            ef, eo, _ = self.bare(o["expansionLoc"])
            return Loc(sf, so, st, True, eo, ef, o["expansionLoc"].get("isMacroArgExpansion", False))
        f, off, tok = self.bare(o)
        if off is None:
            return None
        return Loc(f, off, tok)

    def node(self, n):
        """Decode n's locations, then its children's, in print order"""
        if "loc" in n:
            n["_loc"] = self.loc(n["loc"])
        r = n.get("range")
        if r:
            n["_b"] = self.loc(r.get("begin"))
            n["_e"] = self.loc(r.get("end"))
        for c in n.get("inner", ()):
            self.node(c)
        for c in n.get("array_filler", ()):
            self.node(c)


ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))


def norm(p):
    """A file as one key: absolute (clang names files relative to the
    repository, where it runs), case-folded on Windows"""
    return os.path.normcase(os.path.normpath(os.path.join(ROOT, p))) if p else p


# ---------------------------------------------------------------------------
# Types

_DIMS = re.compile(r"^(.*?)\s*((?:\[\d*\])+)$")
_FN_DIMS = re.compile(r"\(\*((?:\[\d+\])+)\)")


def split_type(qt):
    """(element type, array dims) of a field's type text: 'char *[3]' ->
    ('char *', '[3]'), 'void (*[2])(int)' -> ('void (*)(int)', '[2]')"""
    m = _FN_DIMS.search(qt)
    if m:
        return qt[:m.start()] + "(*)" + qt[m.end():], m.group(1)
    m = _DIMS.match(qt)
    if m and not m.group(1).rstrip().endswith("(*)"):  # 'u8 (*)[3]' points to an array
        return m.group(1), m.group(2)
    return qt, ""


def is_pointer(t):
    t = t.strip()
    return t.endswith("*") or "(*)" in t or t.endswith("* const") or t.endswith("*const")


def spellable(t):
    return "(unnamed" not in t and "(anonymous" not in t


# ---------------------------------------------------------------------------
# One source

# Parameters that take an array of 32-bit pointers as a T ** (a struct of
# pointers, or an array of them, read as list[0], list[1], ...): on
# x86-64 they are pointers to the slots, GRX_EE32 *, and what they reach
# is converted like a field. {function: parameter indexes}
SLOT_PARAMS = {
    # a menu text list: a struct of TxtObj pointers (menuTacticsAiLabel),
    # the first the target, then the parts up to a NULL
    "MenuTextListMeasure": (0,),
    "MenuTextListDraw": (0,),
    # map path splines: x, y, z, e arrays as a struct of pointers (SegIn,
    # SegOut in 5C6AD8) and mapSplineSecondDeriv[3]
    "MapPathSplineResample": (0, 1),
    "MapSplineStepLengths": (1, 3),
    # the video input ring's free space: its pointer out-parameters are
    # u32s in the callers (videoDecBeginPut casts its u32 * to u8 **)
    "viBufBeginPut": (1, 3),
}

CAST_TO = "GRX_EE_AS({p}, "    # GRX_EE_AS(P, x): x as a P
STORE = "GRX_EE_FROM("         # GRX_EE_FROM(v): v's low 32 bits


class Source:
    """The edits for one translation unit"""

    def __init__(self, ast, game_dirs, main_file, placed=frozenset()):
        self.game_dirs = [norm(d) for d in game_dirs]
        self.main = norm(main_file)
        self.fields = {}      # FieldDecl id -> (P, dims, name)
        self.records = {}     # RecordDecl id -> [FieldDecl ids, in order]
        self.typedefs = {}    # typedef name -> record id
        self.tags = {}        # 'struct X' / 'union X' -> record id
        self.decl_edits = {}  # file -> {(begin, end): text}
        self.edits = {}       # file -> [(offset, order, text)] insertions, and (begin, end, text) replacements
        self.ctors = []       # (var name, path, expression text)
        self.const_vars = []  # (file, begin, end) of const tokens to drop
        self.errors = []
        self.text = {}        # file -> bytes (read on demand)
        self.placed = placed  # names at PS2 offsets (bss blocks, linker-defined): extern_array
        Decoder().node(ast)
        self.ast = ast

    # -- files

    def is_game(self, f):
        f = norm(f)
        return f is not None and any(f.startswith(d + os.sep) or f == d for d in self.game_dirs)

    def read(self, f):
        f = norm(f)
        if f not in self.text:
            with open(f, "rb") as fo:
                self.text[f] = fo.read()
        return self.text[f]

    def where(self, n):
        b = n.get("_b") or n.get("_loc")
        if b is None:
            return "?"
        f = b.exp_file if b.macro else b.file
        off = b.exp_off if b.macro else b.off
        try:
            line = self.read(f)[:off].count(b"\n") + 1
        except OSError:
            line = "?"
        return f"{f}:{line}"

    def fail(self, n, msg):
        self.errors.append(f"{self.where(n)}: {msg}")

    # -- pass 1: the fields

    def collect(self, n, parent=None):
        k = n.get("kind")
        if k == "RecordDecl" and n.get("completeDefinition"):
            ids = []
            for c in n.get("inner", ()):
                if c.get("kind") == "FieldDecl":
                    ids.append(c["id"])
                    self.field(n, c)
            self.records[n["id"]] = ids
            if n.get("name"):
                self.tags[f"{n.get('tagUsed', 'struct')} {n['name']}"] = n["id"]
        elif k == "TypedefDecl":
            rid = self.typedef_record(n)
            if rid:
                self.typedefs[n["name"]] = rid
        elif k == "VarDecl":
            self.variable(n)
        elif k == "FunctionDecl" and n.get("name") in SLOT_PARAMS:
            parms = [c for c in n.get("inner", ()) if c.get("kind") == "ParmVarDecl"]
            for i in SLOT_PARAMS[n["name"]]:
                if i < len(parms):
                    self.slot_param(parms[i])
        for c in n.get("inner", ()):
            self.collect(c, n)

    def unnamed_from(self, prev, c):
        if prev is not None and prev.get("kind") == "RecordDecl" and c.get("kind") in ("VarDecl", "FieldDecl"):
            t = c.get("type", {}).get("qualType", "")
            base = split_type(t)[0].strip()
            if "(unnamed" in base:
                self.unnamed[base] = prev["id"]

    def typedef_record(self, n):
        for c in n.get("inner", ()):
            if c.get("kind") == "RecordType":
                return c.get("decl", {}).get("id")
            r = self.typedef_record(c)
            if r:
                return r
        return None

    @staticmethod
    def pointer_type(d):
        """(pointer type, array dims) of a declaration's type, None if it
        isn't a pointer or an array of them"""
        qt = d["type"]["qualType"]
        dq = d["type"].get("desugaredQualType", qt)
        el, dims = split_type(qt)
        del_, ddims = split_type(dq)
        if not is_pointer(el) and not is_pointer(del_):
            return None
        if not dims and ddims:
            dims = ddims
            el = del_
        return (el if is_pointer(el) else del_), dims

    def field(self, rec, f):
        loc = f.get("_loc")
        if loc is None or not self.is_game(loc.file) or loc.macro or f.get("isBitfield"):
            return
        pt = self.pointer_type(f)
        if pt is None:
            return
        p, dims = pt
        self.fields[f["id"]] = (p, dims, f.get("name", ""))
        self.declaration(f, dims, "")

    def declaration(self, d, dims, prefix):
        b, e = d["_b"], d["_e"]
        if b is None or e is None or b.macro or e.macro:
            self.fail(d, f"{d.get('name')} ({d['type']['qualType']}) declared in a macro")
            return
        self.decl_edits.setdefault(norm(b.file), {})[(b.off, e.off + e.tok)] = (d.get("name", ""), dims, prefix)

    def slot_param(self, prm):
        t = prm["type"]["qualType"].strip()
        b, e = prm.get("_b"), prm.get("_e")
        if not self.PTR_PTR.search(t) or b is None or e is None or b.macro or e.macro:
            self.fail(prm, f"slot parameter {prm.get('name', '?')} isn't a plain T **: {t}")
            return
        inner = re.sub(r"\*\s*(?:const\s*|volatile\s*)*$", "", t).strip()
        self.slot_params[prm["id"]] = (inner, prm)
        name = prm.get("name", "")
        loc = prm.get("_loc")
        end = loc.off + len(name) if name and loc is not None and not loc.macro else e.off + e.tok
        self.rep(norm(b.file), b.off, end, "GRX_EE32 *" + name)

    def variable(self, v):
        """A global at a PS2 offset (a bss object, a linker-defined name)
        that is a pointer or an array of them: 32-bit there too, like a
        field. Its declarations in the game's files and in this source."""
        if v.get("name") not in self.placed or not (v.get("storageClass") == "extern" or v.get("_file_scope")):
            return
        pt = self.pointer_type(v)
        if pt is None:
            return
        loc = v.get("_loc")
        if loc is None or loc.macro or not (self.is_game(loc.file) or norm(loc.file) == self.main):
            return
        p, dims = pt
        self.globals[v["name"]] = (p, dims, v["name"])
        self.global_ids.add(v["id"])
        b = v.get("_b")
        text = self.read(b.file)[b.off:loc.off].decode("latin-1") if b is not None and not b.macro else ""
        m = re.match(r"\s*((?:(?:extern|static)\s+)*)", text)
        if b is None or b.macro:
            self.fail(v, f"{v.get('name')} declared in a macro")
            return
        # The declarator only: a definition's range runs on over its
        # initialiser, which is edited apart (scalar_init)
        src = self.read(b.file)
        end = loc.off + len(v["name"])
        if re.match(rb"\s*[)(]", src[end:]):
            # A nested declarator (void (*f)(int)): all of it
            if v.get("init"):
                self.fail(v, f"{v.get('name')}: an initialised function pointer: the rewrite can't")
                return
            end = v["_e"].off + v["_e"].tok
        mm = re.match(rb"(\s*\[[^\]]*\])*", src[end:])
        end += mm.end() if mm else 0
        self.decl_edits.setdefault(norm(b.file), {})[(b.off, end)] = (v["name"], dims, m.group(1) if m else "")

    # -- pass 2: the uses

    def ins(self, file, off, order, text, tag=""):
        """An insertion; at one offset they go by order. Its tag names its
        role, so two that only look alike (a read's ")" and a store's) are
        both kept, and a header's from several sources merge."""
        e = ("i", off, order, text, tag)
        self.edits.setdefault(norm(file), []).append(e)
        return e

    def rep(self, file, b, e, text):
        self.edits.setdefault(norm(file), []).append(("r", b, e, text))

    def span(self, n, ctx=None):
        """(file, begin, end) of n's text where an edit goes; None when it
        can't be edited. ctx is the expression n's use is decided by. In a
        macro: an argument's text where it was written; inside one macro's
        definition, with ctx there too, the definition's text (every use
        of the macro is the same then); the whole of a macro use, with ctx
        outside it, the use's text (NULL, CHR_SENSE(o))."""
        self.last_arg = False
        b, e = n.get("_b"), n.get("_e")
        if b is None or e is None:
            return None
        if not b.macro and not e.macro:
            if norm(b.file) != norm(e.file):
                return None
            return norm(b.file), b.off, e.off + e.tok
        if not (b.macro and e.macro):
            # Starts or ends with a whole macro use (gphRenderCtx->camera,
            # gphRenderCtx a macro): from or to the use's ends
            if b.macro and not b.arg and norm(b.exp_file) == norm(e.file):
                return norm(e.file), b.exp_off, e.off + e.tok
            if e.macro and not e.arg and norm(e.exp_file) == norm(b.file):
                end = self.invocation_end(e.exp_file, e.exp_off)
                if end is not None:
                    return norm(b.file), b.off, end
            return None
        if b.arg and e.arg and norm(b.file) == norm(e.file):
            self.last_arg = True
            return norm(b.file), b.off, e.off + e.tok
        if not b.arg and not e.arg and norm(b.file) == norm(e.file) and ctx is not None \
                and self.in_definition(ctx, b):
            return norm(b.file), b.off, e.off + e.tok
        if b.exp_off == e.exp_off and norm(b.exp_file) == norm(e.exp_file) \
                and (ctx is None or not self.same_expansion(ctx, b)):
            end = self.invocation_end(b.exp_file, b.exp_off)
            if end is not None:
                return norm(b.exp_file), b.exp_off, end
        return None

    def definition(self, f, off):
        """(begin, end) of the #define that offset off of file f is in"""
        text = self.read(f)
        start = text.rfind(b"\n", 0, off) + 1
        while start > 1 and text[start - 2:start] == b"\\\n":
            start = text.rfind(b"\n", 0, start - 2) + 1
        end = text.find(b"\n", off)
        while end > 0 and text[end - 1:end] == b"\\":
            end = text.find(b"\n", end + 1)
        if not re.match(rb"\s*#\s*define\b", text[start:end if end > 0 else len(text)]):
            return None
        return start, end if end > 0 else len(text)

    def in_definition(self, ctx, b):
        cb, ce = ctx.get("_b"), ctx.get("_e")
        if cb is None or ce is None or not (cb.macro and ce.macro) or cb.arg or ce.arg:
            return False
        if norm(cb.file) != norm(b.file) or norm(ce.file) != norm(b.file):
            return False
        d = self.definition(b.file, b.off)
        return d is not None and d[0] <= cb.off and ce.off <= d[1]

    def same_expansion(self, ctx, b):
        """Whether ctx lies within the macro use b is from: both its ends
        (an expression that starts with the use, CUR->v, goes beyond it)"""
        cb, ce = ctx.get("_b"), ctx.get("_e")
        return all(x is not None and x.macro and x.exp_off == b.exp_off and norm(x.exp_file) == norm(b.exp_file)
                   for x in (cb, ce))

    def invocation_end(self, f, off):
        """The end of the macro use at offset off: its name, and its
        arguments' parentheses if it has them"""
        text = self.read(f)
        m = re.match(rb"[A-Za-z_]\w*", text[off:])
        if not m:
            return None
        i = off + m.end()
        j = i
        while j < len(text) and text[j:j + 1] in b" \t\r\n\\":
            j += 1
        if text[j:j + 1] != b"(":
            return i
        depth, k, quote = 0, j, None
        while k < len(text):
            c = text[k:k + 1]
            if quote:
                if c == b"\\":
                    k += 1
                elif c == quote:
                    quote = None
            elif c in (b'"', b"'"):
                quote = c
            elif c == b"(":
                depth += 1
            elif c == b")":
                depth -= 1
                if depth == 0:
                    return k + 1
            k += 1
        return None

    def uses(self, n, stack):
        k = n.get("kind")
        if k == "MemberExpr" and n.get("referencedMemberDecl") in self.fields:
            self.use(n, stack, self.fields[n["referencedMemberDecl"]])
        elif k == "DeclRefExpr" and n.get("referencedDecl", {}).get("id") in self.global_ids:
            self.use(n, stack, self.globals[n["referencedDecl"]["name"]])
        if k == "VarDecl" and n.get("id") in self.global_ids and n.get("init") and \
                not any(c.get("kind") == "InitListExpr" for c in n.get("inner", ())):
            self.scalar_init(n)
        if k == "VarDecl" and n.get("storageClass") == "extern" and n.get("id") not in self.global_ids:
            self.extern_array(n)
        if k == "VarDecl" and n.get("_file_scope") and n.get("storageClass") != "extern":
            self.array_def(n)
        if k in ("ImplicitCastExpr", "CStyleCastExpr") and n.get("castKind") == "BitCast":
            self.struct_as_pointers(n, stack)
            self.ints_as_pointers(n, stack)
        if k in ("UnaryOperator", "ArraySubscriptExpr") and (k != "UnaryOperator" or n.get("opcode") == "*"):
            self.fixed_slot(n, stack)
        if k == "CStyleCastExpr" and n.get("castKind") == "IntegralToPointer" \
                and self.PTR_PTR.search(n["type"]["qualType"]):
            # A fixed address's pointer slots given to a local T **: a slot
            # local (its declaration and dereferences: pass 3)
            if self.address_to_slots(stack, self.pointee(n["type"]["qualType"])):
                self.slot_cast(n)
        if k == "VarDecl" and any(c.get("kind") == "InitListExpr" for c in n.get("inner", ())):
            self.initialiser(n)
        stack.append(n)
        prev = None
        for c in n.get("inner", ()):
            self.unnamed_from(prev, c)
            self.uses(c, stack)
            prev = c
        stack.pop()

    def extern_array(self, var):
        """x86-64 compilers take a global array of 16 bytes or more to be
        16-aligned, and use aligned SSE moves on it; the game's objects sit
        at the PS2's offsets (fixups/objects.py), and its array definitions
        have their element's alignment (array_def), so they may not be. An
        aligned attribute on the declaration replaces that with the
        element's own alignment."""
        t = var.get("type", {}).get("qualType", "").strip()
        if not re.search(r"\[\d+\]$", t):
            return
        loc, e = var.get("_loc"), var.get("_e")
        if loc is None or e is None or loc.macro or e.macro or not self.is_game(loc.file):
            return
        el = split_type(t)[0]   # every dimension at once ('u8 (*)[3]' stays: a pointer)
        if not spellable(el):
            return
        self.ins(norm(e.file), e.off + e.tok, 0, f" __attribute__((aligned(__alignof__({el}))))", "al")

    def array_def(self, var):
        """A global array defined in the game's C (a data file's tables):
        x86-64 compilers align one of 16 bytes or more to 16, leaving gaps
        between arrays the game defines one after another and reads across
        (IpGaugeDraw's &ipDialRects[9 - k] is in ipEnemyRingRects). An
        aligned attribute after the declarator gives the element's own
        alignment, as on i686 and the PS2."""
        t = var.get("type", {}).get("qualType", "").strip()
        loc = var.get("_loc")
        name = var.get("name")
        if not name or not re.search(r"\[\d+\]$", t) or loc is None or loc.macro or not self.is_game(loc.file):
            return
        el = split_type(t)[0]   # every dimension at once ('u8 (*)[3]' stays: a pointer)
        if not spellable(el):
            return
        text = self.read(loc.file)
        if text[loc.off:loc.off + len(name)] != name.encode():
            return
        # After the whole declarator (R (*rows[2])[3] too: inside its
        # parentheses an attribute isn't C): the first =, ; or , outside
        # brackets and the declarator's own parentheses
        k, paren, brack = loc.off + len(name), 0, 0
        while k < len(text):
            c = text[k:k + 1]
            if c == b"(":
                paren += 1
            elif c == b")":
                paren -= 1
            elif c == b"[":
                brack += 1
            elif c == b"]":
                brack -= 1
            elif c in (b"=", b";", b",") and paren <= 0 and brack == 0:
                break
            elif c in (b"{", b"}") and brack == 0 and paren <= 0:
                return
            k += 1
        else:
            return
        end = k
        while end > loc.off and text[end - 1:end].isspace():
            end -= 1
        self.ins(norm(loc.file), end, 0, f" __attribute__((aligned(__alignof__({el}))))", "al")

    def scalar_init(self, var):
        """An initialised definition of such a variable: its value as 32 bits"""
        e = var["inner"][-1]
        s = self.span(e)
        if s is None:
            self.fail(var, f"{var.get('name')} initialised inside a macro")
            return
        fl, b, en = s
        if self.constant(e):
            self.ins(fl, b, -2 * (en - b) - 1, STORE, "st")
            self.ins(fl, en, 2 * (en - b) + 1, ")", "st")
        else:
            self.ctors.append((var["name"], "", self.read(fl)[b:en].decode("latin-1")))
            self.rep(fl, b, en, "0")

    def use(self, m, stack, info):
        p, dims, name = info
        # A pointer to pointers kept in game memory points at 32-bit slots
        # too (a pointer global walking an array of them, a table's
        # pointers to pointer globals): it is read and stepped as a
        # GRX_EE32 *, and what it points at is a converted object (deref)
        inner = None
        if self.PTR_PTR.search(p):
            inner = re.sub(r"\*\s*(?:const\s*|volatile\s*)*$", "", p).strip()
            p = "GRX_EE32 *"
        # The value: the member, or for an array of pointers the element
        v, i = m, len(stack) - 1
        if dims:
            levels = dims.count("[")
            for _ in range(levels):
                if i < 1:
                    break
                cast, sub = stack[i], stack[i - 1]
                if cast.get("kind") == "ImplicitCastExpr" and cast.get("castKind") == "ArrayToPointerDecay" \
                        and sub.get("kind") == "ArraySubscriptExpr" and sub["inner"][0] is cast:
                    v, i = sub, i - 2
                    levels -= 1
                else:
                    break
            if levels:
                # Taken as an array of 32-bit values (s32 *q = m->models,
                # a GRX_EE_PTR(T) * parameter): it is one now
                if levels == dims.count("[") and (self.int32_view(stack[:i + 1])
                                                  or self.address_to_slots(stack[:i + 1], p)
                                                  or self.slot_arith(stack[:i + 1])
                                                  or self.slot_arg(stack[:i + 1])):
                    return
                self.fail(m, f"the array of pointers {name} is used whole (passed or decayed): the rewrite can't")
                return
        # Its context, through parentheses
        top, j = v, i
        while j >= 0 and stack[j].get("kind") == "ParenExpr":
            top, j = stack[j], j - 1
        ctx = stack[j] if j >= 0 else None
        if not spellable(p):
            self.fail(m, f"{name} points to an unnamed type ({p}): the rewrite can't name it")
            return
        kind = ctx.get("kind") if ctx else None
        if kind == "ImplicitCastExpr" and ctx.get("castKind") == "LValueToRValue":
            # What takes the value: the implicit read itself has the
            # member's range, so a macro's definition is only edited when
            # the value's user is in it too
            k2 = j - 1
            while k2 >= 0 and stack[k2].get("kind") in ("ImplicitCastExpr", "ParenExpr"):
                k2 -= 1
            cons = stack[k2] if k2 >= 0 else None
            sv = self.span(v, cons)
            from_arg = self.last_arg
            if sv is None:
                sv = self.span(top, cons)
                from_arg = self.last_arg
                if sv is None:
                    # Left as the 32-bit value where that reads the same
                    if not self.same_unwrapped(stack[:j]):
                        self.fail(m, f"{name} read inside a macro, used as {self.consumer(stack[:j])}: "
                                     f"the rewrite can't place it")
                    return
            f, b, e = sv
            made = [self.ins(f, b, -2 * (e - b), CAST_TO.format(p=p), "rd"), self.ins(f, e, 2 * (e - b), ")", "rd")]
            if from_arg:
                # A macro argument the macro may also assign (RELOC(x):
                # (u32)(x), then (x) = ...): settled once all uses are seen
                self.arg_reads.setdefault((f, b, e), []).append((m, self.same_unwrapped(stack[:j]), made))
            if inner is not None:
                self.deref(stack[:j], inner, name, ctx)
                # kept in a local T ** (p = sys->gens): that local is a
                # pointer to the slots too
                self.address_to_slots(stack[:j], inner)
            return
        if kind in ("BinaryOperator", "CompoundAssignOperator", "UnaryOperator") \
                and ctx.get("opcode") in ("=", "+=", "-=", "++", "--"):
            sm = self.span(v, ctx)
            if self.last_arg:
                self.arg_lvalues.add(sm)
        st = self.span(top, ctx)  # with its parentheses: the operand of ++, +=, =
        if kind == "BinaryOperator" and ctx.get("opcode") == "=" and ctx["inner"][0] is top:
            rhs = self.span(ctx["inner"][1], ctx)
            if rhs is None:
                # Assigning the pointer to the 32-bit field converts it the
                # same way: the wrap only spells it out
                self.assignment_value(ctx, stack[:j], name)
                return
            f, b, e = rhs
            self.ins(f, b, -2 * (e - b) - 1, STORE, "st")
            self.ins(f, e, 2 * (e - b) + 1, ")", "st")
            self.assignment_value(ctx, stack[:j], name)
        elif kind == "CompoundAssignOperator" and ctx["inner"][0] is top and ctx.get("opcode") in ("+=", "-="):
            rhs = self.span(ctx["inner"][1], ctx)
            sv = st
            if sv is None or rhs is None:
                self.fail(m, f"{name} {ctx.get('opcode')} inside a macro: the rewrite can't place it")
                return
            f, b, e = sv
            rf, rb, re_ = rhs
            if rf != f:
                self.fail(m, f"{name} {ctx.get('opcode')} across files")
                return
            sign = "+" if ctx["opcode"] == "+=" else "-"
            self.ins(f, b, -2 * (re_ - b) - 1, f"GRX_EE_ADD({p}, ", "ad")
            self.rep(f, e, rb, f", {sign}(")
            self.ins(f, re_, 2 * (re_ - b) + 1, "))", "ad")
        elif kind == "UnaryOperator" and ctx.get("opcode") in ("++", "--"):
            sv = st
            if sv is None:
                self.fail(m, f"{name}{ctx['opcode']} inside a macro: the rewrite can't place it")
                return
            f, b, e = sv
            d = "1" if ctx["opcode"] == "++" else "-1"
            cs = self.span(ctx)
            if cs is None:
                self.fail(m, f"{name}{ctx['opcode']} inside a macro: the rewrite can't place it")
                return
            _, cb, ce = cs
            if ctx.get("isPostfix"):
                self.ins(f, b, -2 * (ce - b) - 1, f"GRX_EE_POSTADD({p}, ", "pi")
                self.rep(f, e, ce, f", {d})")
            else:
                self.rep(f, cb, b, f"GRX_EE_ADD({p}, ")
                self.ins(f, e, 2 * (e - cb) + 1, f", {d})", "pr")
            if inner is not None:
                self.deref(stack[:j], inner, name, ctx)
        elif kind == "UnaryExprOrTypeTraitExpr":
            pass  # sizeof, alignof: 4 now, as on the PS2
        elif kind == "UnaryOperator" and ctx.get("opcode") == "&":
            if not self.address_as_32(stack[:j]) and not self.address_to_slots(stack[:j], p):
                self.fail(m, f"the address of {name} is taken: it is a 32-bit EE address now, not a {p}")
        else:
            self.fail(m, f"{name} used as {kind}{' ' + ctx.get('opcode', '') if ctx else ''}: the rewrite can't")

    SAFE_OPS = {"==", "!=", "<", ">", "<=", ">=", "&&", "||", ",", "="}

    def consumer(self, above):
        """The expression that takes a value (above: its ancestors)"""
        i = len(above) - 1
        while i >= 0 and above[i].get("kind") == "ParenExpr":
            i -= 1
        if i < 0:
            return "?"
        n = above[i]
        return n.get("kind") + (" " + n["opcode"] if "opcode" in n else "")

    def same_unwrapped(self, above):
        """Whether a field's read, left as its 32-bit value, does what the
        pointer did: converted (a cast, an argument, an initialiser, a
        pointer variable assigned), compared, tested. Not arithmetic (+ -
        would count bytes, not elements); a dereference, subscript, member
        or call through it doesn't compile, so it can't go unnoticed."""
        i = len(above) - 1
        while i >= 0 and above[i].get("kind") == "ParenExpr":
            i -= 1
        if i < 0:
            return False
        n = above[i]
        k = n.get("kind")
        if k in ("CStyleCastExpr", "ImplicitCastExpr", "CallExpr", "VarDecl", "InitListExpr", "ReturnStmt",
                 "IfStmt", "WhileStmt", "DoStmt", "ForStmt", "ConditionalOperator", "SwitchStmt"):
            return True
        if k == "BinaryOperator" and n.get("opcode") in self.SAFE_OPS:
            return True
        if k == "UnaryOperator" and n.get("opcode") == "!":
            return True
        return False

    INT32_PTR = re.compile(r"^(?:(?:const|volatile)\s+)*(?:s32|u32|int|unsigned int|signed int|long|unsigned long)"
                           r"(?:\s+(?:const|volatile))*\s*(?:\*|\(\*\)(?:\[\d+\])+)$")
    # A byte pointer: an address worked out inside a block of data
    # ((T **)((u8 *)msg + msg->strOfs)), whose pointers are 32-bit too
    BYTE_PTR = re.compile(r"^(?:(?:const|volatile)\s+)*(?:u8|s8|char|unsigned char|signed char)"
                          r"(?:\s+(?:const|volatile))*\s*\*$")
    INT_TYPE = re.compile(r"^(?:(?:const|volatile)\s+)*(?:[su](?:8|16|32|64)|(?:unsigned |signed )?(?:char|short|int|long)"
                          r"(?: long)?|unsigned|__UINTPTR_TYPE__|uintptr_t)$")

    def int32_view(self, above):
        """Whether a value (above: its ancestors, the nearest last) is taken
        as a pointer to 32-bit integers: converted to one, explicitly or
        implicitly (an argument to such a parameter), assigned to or
        initialising such a variable"""
        i = len(above) - 1
        while i >= 0:
            n = above[i]
            k = n.get("kind")
            t = n.get("type", {}).get("qualType", "").strip()
            if k == "ParenExpr":
                pass
            elif k in ("ImplicitCastExpr", "CStyleCastExpr"):
                if self.INT32_PTR.match(t):
                    return True
                if k == "CStyleCastExpr" or n.get("castKind") not in ("ArrayToPointerDecay", "NoOp", "BitCast"):
                    return False
            elif k == "VarDecl" or (k == "BinaryOperator" and n.get("opcode") == "="):
                return bool(self.INT32_PTR.match(t))
            else:
                return False
            i -= 1
        return False

    PTR_PTR = re.compile(r"\*\s*(?:const\s*|volatile\s*)*\*\s*(?:const\s*|volatile\s*)*$")

    def slot_arg(self, above):
        """Whether a value (above: its ancestors) is an argument to a slot
        parameter (SLOT_PARAMS)"""
        i = len(above) - 1
        child = None
        while i >= 0 and above[i].get("kind") in ("ParenExpr", "ImplicitCastExpr", "CStyleCastExpr", "UnaryOperator") \
                and (above[i].get("kind") != "UnaryOperator" or above[i].get("opcode") == "&"):
            child = above[i]
            i -= 1
        if i < 0 or above[i].get("kind") != "CallExpr":
            return False
        call = above[i]
        callee = call["inner"][0]
        while callee.get("kind") in ("ImplicitCastExpr", "ParenExpr") and callee.get("inner"):
            callee = callee["inner"][0]
        fname = callee.get("referencedDecl", {}).get("name")
        if fname not in SLOT_PARAMS:
            return False
        args = call["inner"][1:]
        return any(a is child for idx, a in enumerate(args) if idx in SLOT_PARAMS[fname])

    def slot_cast(self, cast):
        """(T **)0x70002800 -> (GRX_EE32 *)0x70002800: a pointer slot at a
        fixed EE address (the scratchpad's) is 4 bytes"""
        b = cast.get("_b")
        if b is None:
            return False
        if b.macro and (b.arg or self.definition(b.file, b.off) is None):
            return False
        # The type in parentheses, from the cast's own text (its operand may
        # be another macro, spelled elsewhere)
        text = self.read(b.file)
        if text[b.off:b.off + 1] != b"(":
            return False
        depth, k = 0, b.off
        while k < len(text):
            c = text[k:k + 1]
            if c == b"(":
                depth += 1
            elif c == b")":
                depth -= 1
                if depth == 0:
                    break
            elif c == b"\n" and text[k - 1:k] != b"\\":
                return False
            k += 1
        else:
            return False
        vol = "volatile " if "volatile" in cast["type"]["qualType"] else ""
        self.rep(norm(b.file), b.off, k + 1, f"({vol}GRX_EE32 *)")
        return True

    def fixed_slot(self, n, stack):
        """*(T **)0x700000EC, ((T **)addr)[i]: a pointer kept at a fixed EE
        address (the scratchpad's), 32 bits on the PS2, so here too"""
        op = n["inner"][0]
        while op.get("kind") in ("ParenExpr", "ImplicitCastExpr") and op.get("inner"):
            if op.get("kind") == "ImplicitCastExpr" and op.get("castKind") not in ("LValueToRValue", "NoOp"):
                break
            op = op["inner"][0]
        if op.get("kind") != "CStyleCastExpr":
            return
        t = op["type"]["qualType"]
        if not self.PTR_PTR.search(t):
            return
        if op.get("castKind") == "BitCast":
            # A pointer to 32-bit integers viewed as pointers (*(T **)&s->s32field,
            # ((T **)file->offsets)[i]), or a byte pointer into a block of
            # data (((T **)((u8 *)msg + ofs))[i]): 32-bit slots too
            src = op["inner"][0]["type"]["qualType"].strip() if op.get("inner") else ""
            if not self.INT32_PTR.match(src) and not self.BYTE_PTR.match(src):
                return
        elif op.get("castKind") != "IntegralToPointer":
            return
        self.handled_casts.add(op["id"])
        if not self.slot_cast(op):
            self.fail(n, f"a pointer slot at a fixed address ({t}) in a macro: the rewrite can't")
            return
        self.use(n, stack, (self.pointee(t), "", "*" + t))

    def ints_as_pointers(self, cast, stack):
        """A pointer to 32-bit integers or a byte pointer taken as a T **
        (videoDecBeginPut's (u8 **)arg1, a u32 out-parameter; a message
        bank's string table at a byte offset): what goes through it would
        be 8 bytes. Fine into a slot parameter or a slot local; else a fixup."""
        t = cast["type"]["qualType"].strip()
        if not self.PTR_PTR.search(t) or not cast.get("inner") or cast["id"] in self.handled_casts:
            return
        src = cast["inner"][0]["type"]["qualType"].strip()
        if not self.INT32_PTR.match(src) and not self.BYTE_PTR.match(src):
            return
        if self.slot_arg(list(stack) + [cast]) or self.address_to_slots(list(stack), self.pointee(t)):
            return
        self.fail(cast, f"a {src} (32-bit slots) taken as a {t}: what goes through it would be 8 bytes")

    def struct_as_pointers(self, cast, stack=()):
        """A struct whose pointer fields are 32-bit now, viewed as an array
        of pointers (&si as an f32 **, for a function that reads in[0],
        in[1]): those reads would take 8 bytes. Found here, settled by a
        fixup (the reader takes GRX_EE_PTR(T) *)."""
        t = cast["type"]["qualType"].strip()
        if not self.PTR_PTR.search(t) or not cast.get("inner"):
            return
        src = cast["inner"][0]["type"]["qualType"].strip()
        if not src.endswith("*") or self.PTR_PTR.search(src):
            return
        rid = self.record_of(src[:-1].strip())
        if rid is None or not any(f in self.fields for f in self.records.get(rid, [])):
            return
        if self.slot_arg(list(stack) + [cast]):
            return
        self.fail(cast, f"a {src} (a struct with 32-bit pointer fields) taken as a {t}: its reader takes 8-byte pointers")

    @staticmethod
    def pointee(t):
        """'s32 **' -> 's32 *'"""
        return re.sub(r"\*\s*(?:const\s*|volatile\s*)*$", "", t.strip()).strip()

    def slot_arith(self, above):
        """An array of 32-bit pointers decayed into arithmetic or a
        comparison with another pointer to them (listPtr - list), or under
        sizeof: both sides are GRX_EE32 * after the rewrite"""
        i = len(above) - 1
        while i >= 0 and above[i].get("kind") in ("ParenExpr", "ImplicitCastExpr"):
            i -= 1
        if i < 0:
            return False
        n = above[i]
        if n.get("kind") == "UnaryExprOrTypeTraitExpr":
            return True
        return n.get("kind") == "BinaryOperator" and n.get("opcode") in ("-", "==", "!=", "<", ">", "<=", ">=")

    def deref(self, above, inner, name, read=None):
        """A pointer to 32-bit slots, read (above: the read's ancestors):
        what a * or [] on it reaches is a converted object of type inner"""
        i = len(above) - 1
        while i >= 0 and above[i].get("kind") in ("ParenExpr", "ImplicitCastExpr", "CStyleCastExpr"):
            i -= 1
        if i < 0:
            return
        n = above[i]
        child = above[i + 1] if i + 1 < len(above) else read
        if (n.get("kind") == "UnaryOperator" and n.get("opcode") == "*") or \
                (n.get("kind") == "ArraySubscriptExpr" and child is n["inner"][0]):
            self.use(n, above[:i], (inner, "", f"*{name}"))

    def address_to_slots(self, above, p):
        """The address of 32-bit pointer slots (&obj, a decayed array of
        them) going where that is fine: into a local T ** (which becomes a
        pointer to the slots: slot_local), cast to a game struct's pointer
        (whose pointer fields are 32-bit too: the decompilation's view of
        consecutive globals), or as raw memory (void *: MemSet, a buffer)"""
        i = len(above) - 1
        while i >= 0:
            n = above[i]
            k = n.get("kind")
            t = n.get("type", {}).get("qualType", "").strip()
            if k == "ParenExpr" or (k == "UnaryOperator" and n.get("opcode") == "&") or \
                    (k == "BinaryOperator" and n.get("opcode") in ("+", "-") and self.PTR_PTR.search(t)):
                i -= 1
                continue
            if k in ("ImplicitCastExpr", "CStyleCastExpr"):
                if re.fullmatch(r"(?:const |volatile )*void \*", t) and k == "ImplicitCastExpr":
                    return True
                if k == "CStyleCastExpr" and t.endswith("*") and not self.PTR_PTR.search(t) \
                        and self.record_of(t[:-1].strip()) is not None:
                    return True
                i -= 1
                continue
            if k == "VarDecl" and not n.get("_file_scope") and n.get("storageClass") not in ("extern", "static") \
                    and self.PTR_PTR.search(t):
                self.slot_locals[n["id"]] = (self.pointee(t), n)  # its own type's pointee
                return True
            if k == "InitListExpr":
                return True  # into a converted field of a table: a pointer to slots there (use)
            if k == "BinaryOperator" and n.get("opcode") == "=" and self.PTR_PTR.search(t):
                lhs = n["inner"][0]
                while lhs.get("kind") in ("ParenExpr", "ImplicitCastExpr") and lhs.get("inner"):
                    lhs = lhs["inner"][0]
                if (lhs.get("kind") == "MemberExpr" and lhs.get("referencedMemberDecl") in self.fields) or \
                        lhs.get("referencedDecl", {}).get("id") in self.global_ids:
                    return True  # into a converted pointer to pointers: slots too (use)
                d = lhs.get("referencedDecl", {})
                if lhs.get("kind") == "DeclRefExpr" and d.get("kind") == "VarDecl":
                    self.slot_assigned[d["id"]] = self.pointee(t)
                    return True
            return False
        return False

    def slots(self, n, stack, decls):
        """Pass 3: the slot locals. Their declarations become pointers to
        32-bit slots; a dereference of one (*q, q[i], *q++) is a converted
        object, like a field."""
        k = n.get("kind")
        if k == "VarDecl" and n.get("id") in decls and n.get("id") not in self.slot_params:
            p, _ = decls[n["id"]]
            b, loc = n.get("_b"), n.get("_loc")
            if b is None or loc is None or b.macro or loc.macro:
                self.fail(n, f"{n.get('name')}: a pointer to 32-bit slots declared in a macro")
            else:
                self.rep(norm(b.file), b.off, loc.off + len(n["name"]), "GRX_EE32 *" + n["name"])
        target = None
        if k == "UnaryOperator" and n.get("opcode") == "*":
            target = n["inner"][0]
        elif k == "ArraySubscriptExpr":
            target = n["inner"][0]
        if target is not None:
            while target.get("kind") in ("ImplicitCastExpr", "ParenExpr", "CStyleCastExpr") or \
                    (target.get("kind") == "UnaryOperator" and target.get("opcode") in ("++", "--")):
                if not target.get("inner"):
                    break
                target = target["inner"][0]
            d = target.get("referencedDecl", {}) if target.get("kind") == "DeclRefExpr" else {}
            if d.get("id") in decls:
                p = decls[d["id"]][0]
                self.use(n, stack, (p, "", d.get("name", "*")))
        if k == "DeclRefExpr" and n.get("referencedDecl", {}).get("id") in decls:
            # Given to another local T ** (pp = arg0 + 1): slots too
            self.address_to_slots(stack, decls[n["referencedDecl"]["id"]][0])
        stack.append(n)
        for c in n.get("inner", ()):
            self.slots(c, stack, decls)
        stack.pop()

    def address_as_32(self, above):
        """A field's address used as a 32-bit integer's ((s32 *)&p->f, the
        decompilation's way to load or store it as an int; memcpy of 4
        bytes): the 32-bit field is just that"""
        i = len(above) - 1
        while i >= 0 and above[i].get("kind") == "ParenExpr":
            i -= 1
        if i < 0:
            return False
        if self.int32_view(above):
            return True
        # Its address as a number (the uncached alias, (u32)&p->f | 0x20000000)
        if above[i].get("kind") == "CStyleCastExpr" and self.INT_TYPE.match(above[i]["type"]["qualType"].strip()):
            return True
        # memcpy / memmove(&p->f, src, 4) or (dst, &p->f, 4)
        while i >= 0 and above[i].get("kind") in ("ImplicitCastExpr", "ParenExpr"):
            i -= 1
        if i >= 0 and above[i].get("kind") == "CallExpr":
            call = above[i]
            callee = call["inner"][0]
            while callee.get("kind") in ("ImplicitCastExpr", "ParenExpr") and callee.get("inner"):
                callee = callee["inner"][0]
            name = callee.get("referencedDecl", {}).get("name")
            size = call["inner"][-1]
            while size.get("kind") in ("ImplicitCastExpr", "ParenExpr") and size.get("inner"):
                size = size["inner"][0]
            if name in ("memcpy", "memmove", "__builtin_memcpy") and size.get("kind") == "IntegerLiteral" \
                    and size.get("value") == "4":
                return True
        return False

    def assignment_value(self, op, above, name):
        """An assignment's value is the stored u32 now: fine as a statement
        or assigned on, not as a pointer in an expression"""
        parent = above[-1] if above else None
        while parent is not None and parent.get("kind") == "ParenExpr":
            above = above[:-1]
            parent = above[-1] if above else None
        k = parent.get("kind") if parent else None
        ok = {"CompoundStmt", "IfStmt", "ForStmt", "WhileStmt", "DoStmt", "LabelStmt", "CaseStmt",
              "DefaultStmt", "SwitchStmt", None}
        if k in ok or (k == "BinaryOperator" and parent.get("opcode") in self.SAFE_OPS) or \
                (k == "ImplicitCastExpr" and parent.get("castKind") == "ToVoid") or \
                (k == "CStyleCastExpr" and parent["type"]["qualType"] == "void"):
            return
        self.fail(op, f"the value of an assignment to {name} is used ({k}): it is the 32-bit value now")

    # -- initialisers

    def record_of(self, qt):
        qt = qt.strip()
        for pre in ("const ", "volatile "):
            if qt.startswith(pre):
                qt = qt[len(pre):]
        if qt in self.typedefs:
            return self.typedefs[qt]
        return self.tags.get(qt, self.unnamed.get(qt))

    def initialiser(self, var):
        init = next((c for c in var.get("inner", ()) if c.get("kind") == "InitListExpr"), None)
        if init is None:
            return
        static = var.get("storageClass") in ("static", "extern") or var.get("_file_scope", False)
        found = []
        self.walk_init(init, var["type"]["qualType"], "", found)
        if not found:
            return
        for f, path, e in found:
            if not static:
                s = self.span(e)
                if s is None:
                    self.fail(e, f"{var.get('name')}{path} initialised inside a macro")
                    continue
                fl, b, en = s
                self.ins(fl, b, -2 * (en - b) - 1, STORE, "st")
                self.ins(fl, en, 2 * (en - b) + 1, ")", "st")
                continue
            if self.constant(e):
                s = self.span(e)
                if s is None:
                    self.fail(e, f"{var.get('name')}{path} initialised inside a macro")
                    continue
                fl, b, en = s
                self.ins(fl, b, -2 * (en - b) - 1, STORE, "st")
                self.ins(fl, en, 2 * (en - b) + 1, ")", "st")
                continue
            s = self.span(e)
            if s is None:
                self.fail(e, f"{var.get('name')}{path} initialised inside a macro")
                continue
            fl, b, en = s
            text = self.read(fl)[b:en].decode("latin-1")
            self.rep(fl, b, en, "0")
            self.ctors.append((var["name"], path, text))
        if any(not self.constant(e) for _, _, e in found) and static and "const" in var["type"]["qualType"]:
            self.drop_const(var)

    def walk_init(self, il, qt, path, found):
        base, dims = split_type(qt)
        items = [c for c in il.get("inner", ())]
        if dims:
            first = dims[dims.index("["):dims.index("]") + 1]
            rest = dims[len(first):]
            el = base + rest if rest else base
            for i, c in enumerate(items):
                self.init_item(c, el, f"{path}[{i}]", found)
            return
        rid = self.record_of(qt)
        if rid is None:
            return
        fids = [f for f in self.records.get(rid, []) if self._names.get(f)]  # unnamed bit-fields take no initialiser
        if "field" in il:  # a union: one member
            fid = il["field"].get("id")
            if items:
                self.init_field(items[0], fid, path, found)
            return
        for fid, c in zip(fids, items):
            self.init_field(c, fid, path, found)

    def init_field(self, c, fid, path, found):
        if fid in self.fields:
            p, dims, name = self.fields[fid]
            if dims:
                if c.get("kind") == "InitListExpr":
                    for i, e in enumerate(c.get("inner", ())):
                        self.init_value(e, f"{path}.{name}[{i}]", found)
                return
            self.init_value(c, f"{path}.{name}", found)
            return
        # Not one: a nested struct or array may hold some
        name = self.field_name(fid)
        if c.get("kind") == "InitListExpr" and name is not None:
            self.init_item(c, c["type"]["qualType"], f"{path}.{name}", found)

    def field_name(self, fid):
        return self._names.get(fid) if hasattr(self, "_names") else None

    def init_item(self, c, qt, path, found):
        if c.get("kind") == "InitListExpr":
            self.walk_init(c, c["type"]["qualType"], path, found)

    def init_value(self, e, path, found):
        if e.get("kind") == "ImplicitValueInitExpr":
            return
        found.append((None, path, e))

    def constant(self, e):
        """No address in it: an integer constant, NULL"""
        def has_addr(n):
            k = n.get("kind")
            if k in ("StringLiteral", "AddrLabelExpr"):
                return True
            if k == "DeclRefExpr" and n.get("referencedDecl", {}).get("kind") in ("VarDecl", "FunctionDecl"):
                return True
            return any(has_addr(c) for c in n.get("inner", ()))
        return not has_addr(e)

    def drop_const(self, var):
        b, loc = var.get("_b"), var.get("_loc")
        if b is None or loc is None or b.macro or loc.macro:
            self.fail(var, f"const {var.get('name')} needs a constructor, in a macro")
            return
        f = norm(b.file)
        text = self.read(f)
        for m in re.finditer(rb"\bconst\b", text[b.off:loc.off]):
            self.rep(f, b.off + m.start(), b.off + m.end(), "")

    # -- run

    def run(self):
        self._names = {}
        self.unnamed = {}
        self.globals = {}     # name -> (P, dims, name): variables made 32-bit
        self.slot_params = {}  # ParmVarDecl id -> (inner pointer type, node): SLOT_PARAMS
        self.global_ids = set()  # their declarations
        for top in self.ast.get("inner", ()):
            if top.get("kind") == "VarDecl":
                top["_file_scope"] = True

        def names(n):
            if n.get("kind") == "FieldDecl":
                self._names[n["id"]] = n.get("name")
            for c in n.get("inner", ()):
                names(c)
        names(self.ast)
        self.collect(self.ast)
        for top in self.ast.get("inner", ()):
            if top.get("kind") == "VarDecl":
                top["_file_scope"] = True
        self.arg_reads, self.arg_lvalues = {}, set()
        self.slot_locals, self.slot_assigned = {}, {}
        self.handled_casts = set()
        self.uses(self.ast, [])
        decls = dict(self.slot_locals)
        missing = set(self.slot_assigned) - set(decls)
        if missing:
            # Assigned, not initialised: their declarations, found by id
            def find(n):
                if n.get("kind") == "VarDecl" and n.get("id") in missing:
                    decls[n["id"]] = (self.slot_assigned[n["id"]], n)
                for c in n.get("inner", ()):
                    find(c)
            find(self.ast)
        decls.update({k: v for k, v in self.slot_params.items()})
        done = set()
        while set(decls) - done:
            todo = {k: v for k, v in decls.items() if k not in done}
            done |= set(todo)
            self.slots(self.ast, [], todo)
            # locals that took an element's address in this round
            decls.update({k: v for k, v in self.slot_locals.items() if k not in decls})
            assigned = {k: v for k, v in self.slot_assigned.items() if k not in decls}
            if assigned:
                def find2(n):
                    if n.get("kind") == "VarDecl" and n.get("id") in assigned:
                        decls[n["id"]] = (assigned[n["id"]], n)
                    for c in n.get("inner", ()):
                        find2(c)
                find2(self.ast)
        for sp, reads in self.arg_reads.items():
            if sp not in self.arg_lvalues:
                continue
            # The macro assigns the argument too: its text must stay an
            # lvalue, so its reads stay the 32-bit value, where that reads
            # the same
            for m, safe, made in reads:
                for e in made:
                    self.edits[sp[0]].remove(e)
                if not safe:
                    self.fail(m, "a macro argument read as a pointer and assigned too: the rewrite can't")
        return self


def decl_text(name, dims):
    return f"GRX_EE32 {name}{dims}"


def apply(text, edits):
    """text (bytes) with edits: ("i", offset, order, s) insertions and
    ("r", begin, end, s) replacements. Insertions at one offset go by
    order (an outer expression's opening first, its closing last)."""
    ins = sorted({tuple(e) for e in edits if e[0] == "i"}, key=lambda e: (e[1], e[2], e[3], e[4]))
    reps = sorted({(e[1], e[2], e[3]) for e in edits if e[0] == "r"})
    for a, b in zip(reps, reps[1:]):
        if a[1] > b[0]:
            raise Fail(f"overlapping replacements at {a[0]}..{a[1]} and {b[0]}..{b[1]}")
    out, pos = [], 0
    ii = 0
    for b, e, s in reps + [(len(text), len(text), None)]:
        while ii < len(ins) and ins[ii][1] <= b:
            off, s2 = ins[ii][1], ins[ii][3]
            out.append(text[pos:off])
            out.append(s2.encode("latin-1"))
            pos = off
            ii += 1
        if s is None:
            break
        out.append(text[pos:b])
        out.append(s.encode("latin-1"))
        pos = e
        # insertions inside a replaced range are dropped with it
        while ii < len(ins) and ins[ii][1] < e:
            ii += 1
    out.append(text[pos:])
    return b"".join(out)


# ---------------------------------------------------------------------------
# Driver for one translation unit (run in a worker process)

def analyse(job):
    """job: (clang, triple, src path, flags, game dirs, cache path, key).
    Returns the edits as plain data: {"decls": {file: [[b, e, name, dims]]},
    "edits": {file: [...]}, "ctors": [...], "errors": [...]}"""
    clang, triple, src, flags, game_dirs, cache, key = job[:7]
    placed = placed_names(job[7]) if len(job) > 7 else frozenset()
    if cache and os.path.exists(cache):
        try:
            with open(cache) as f:
                c = json.load(f)
            if c.get("key") == key:
                return c["result"]
        except (OSError, ValueError):
            pass
    p = subprocess.run([clang, "-target", triple, "-fsyntax-only", "-w", "-Xclang", "-ast-dump=json", src] + flags,
                       capture_output=True, text=True, encoding="utf-8", errors="replace", cwd=ROOT)
    if p.returncode != 0:
        result = {"decls": {}, "edits": {}, "ctors": [], "failed": True,
                  "errors": [f"{src}: clang failed: {p.stderr[:1500]}"]}
    else:
        s = Source(json.loads(p.stdout), game_dirs, src, placed).run()
        decls = {f: [[b, e, v[0], v[1], v[2]] for (b, e), v in d.items()] for f, d in s.decl_edits.items()}
        result = {"decls": decls, "edits": s.edits, "ctors": s.ctors, "errors": s.errors}
    if cache:
        os.makedirs(os.path.dirname(cache), exist_ok=True)
        with open(cache, "w") as f:
            json.dump({"key": key, "result": result}, f)
    return result


_placed = {}


def placed_names(path):
    """The names in the file (one a line), read once per process"""
    if path not in _placed:
        with open(path) as f:
            _placed[path] = frozenset(f.read().split())
    return _placed[path]


def ctor_text(ctors, tag):
    if not ctors:
        return ""
    lines = ["", "/* port/tools/ee32.py: addresses for 32-bit pointer fields, stored at start-up */",
             f"static void __attribute__((constructor)) grx_ee32_init_{tag}(void) {{"]
    for var, path, expr in ctors:
        lines.append(f"    {var}{path} = GRX_EE_FROM({expr});")
    lines.append("}")
    return "\n".join(lines) + "\n"


def key_of(src_text, headers_sig, flags):
    return hashlib.sha1(src_text + headers_sig.encode() + " ".join(flags).encode() + str(VERSION).encode()).hexdigest()


# ---------------------------------------------------------------------------
# All of them: merge, apply, write

def merge(results, texts):
    """results: {source: analyse() result}. Returns ({file: [edits]},
    {source: ctors}, [errors]). Edits are deduplicated (a header's come
    from every source that includes it); a field's declaration is replaced
    with those that share its declaration (`T *a, *b;`)."""
    errors = []
    edits = {}
    ctors = {}
    decls = {}
    for src, r in results.items():
        errors.extend(r["errors"])
        for f, el in r["edits"].items():
            edits.setdefault(f, set()).update(tuple(e) for e in el)
        for f, dl in r["decls"].items():
            for b, e, name, dims, prefix in dl:
                decls.setdefault(f, {})[(b, e)] = (name, dims, prefix)
        if r["ctors"]:
            ctors[src] = r["ctors"]
    for f, d in decls.items():
        text = texts(f)
        groups = {}
        for (b, e), v in d.items():
            groups.setdefault(b, []).append((e, v))
        for b, members in groups.items():
            members.sort()
            end = members[-1][0]
            after = text[end:end + 200].lstrip()
            if not after.startswith(b";") and not after.startswith(b"="):
                line = text[:b].count(b"\n") + 1
                errors.append(f"{f}:{line}: a pointer field declared with other fields that aren't pointers "
                              f"({text[b:end + 40].decode('latin-1').strip()!r}): split it with a header fixup")
                continue
            new = members[0][1][2] + "GRX_EE32 " + ", ".join(name + dims for _, (name, dims, _) in members)
            edits.setdefault(f, set()).add(("r", b, end, new))
    return {f: sorted(v) for f, v in edits.items()}, ctors, errors
