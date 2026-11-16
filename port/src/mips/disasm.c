/* A small R5900 disassembler for the interpreter's messages
   (port/include/grx_mips.h): mnemonics for the integer, FPU and common
   COP2 instructions, the raw fields for the rest. */
#include <stdio.h>
#include "grx_mips.h"

static const char *const R[32] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2",
                                  "t3",   "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5",
                                  "s6",   "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};

static const char *const OPS[64] = {
    [0x02] = "j",
    [0x03] = "jal",
    [0x04] = "beq",
    [0x05] = "bne",
    [0x06] = "blez",
    [0x07] = "bgtz",
    [0x08] = "addi",
    [0x09] = "addiu",
    [0x0A] = "slti",
    [0x0B] = "sltiu",
    [0x0C] = "andi",
    [0x0D] = "ori",
    [0x0E] = "xori",
    [0x0F] = "lui",
    [0x14] = "beql",
    [0x15] = "bnel",
    [0x16] = "blezl",
    [0x17] = "bgtzl",
    [0x18] = "daddi",
    [0x19] = "daddiu",
    [0x1A] = "ldl",
    [0x1B] = "ldr",
    [0x1E] = "lq",
    [0x1F] = "sq",
    [0x20] = "lb",
    [0x21] = "lh",
    [0x22] = "lwl",
    [0x23] = "lw",
    [0x24] = "lbu",
    [0x25] = "lhu",
    [0x26] = "lwr",
    [0x27] = "lwu",
    [0x28] = "sb",
    [0x29] = "sh",
    [0x2A] = "swl",
    [0x2B] = "sw",
    [0x2C] = "sdl",
    [0x2D] = "sdr",
    [0x2E] = "swr",
    [0x2F] = "cache",
    [0x31] = "lwc1",
    [0x33] = "pref",
    [0x36] = "lqc2",
    [0x37] = "ld",
    [0x39] = "swc1",
    [0x3E] = "sqc2",
    [0x3F] = "sd",
};

static const char *const SPECIAL[64] = {
    [0x00] = "sll",
    [0x02] = "srl",
    [0x03] = "sra",
    [0x04] = "sllv",
    [0x06] = "srlv",
    [0x07] = "srav",
    [0x08] = "jr",
    [0x09] = "jalr",
    [0x0A] = "movz",
    [0x0B] = "movn",
    [0x0C] = "syscall",
    [0x0D] = "break",
    [0x0F] = "sync",
    [0x10] = "mfhi",
    [0x11] = "mthi",
    [0x12] = "mflo",
    [0x13] = "mtlo",
    [0x14] = "dsllv",
    [0x16] = "dsrlv",
    [0x17] = "dsrav",
    [0x18] = "mult",
    [0x19] = "multu",
    [0x1A] = "div",
    [0x1B] = "divu",
    [0x20] = "add",
    [0x21] = "addu",
    [0x22] = "sub",
    [0x23] = "subu",
    [0x24] = "and",
    [0x25] = "or",
    [0x26] = "xor",
    [0x27] = "nor",
    [0x28] = "mfsa",
    [0x29] = "mtsa",
    [0x2A] = "slt",
    [0x2B] = "sltu",
    [0x2C] = "dadd",
    [0x2D] = "daddu",
    [0x2E] = "dsub",
    [0x2F] = "dsubu",
    [0x30] = "tge",
    [0x31] = "tgeu",
    [0x32] = "tlt",
    [0x33] = "tltu",
    [0x34] = "teq",
    [0x36] = "tne",
    [0x38] = "dsll",
    [0x3A] = "dsrl",
    [0x3B] = "dsra",
    [0x3C] = "dsll32",
    [0x3E] = "dsrl32",
    [0x3F] = "dsra32",
};

static const char *const FPU_S[64] = {
    "add.s", "sub.s", "mul.s", "div.s", "sqrt.s", "abs.s", "mov.s", "neg.s", 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, "rsqrt.s", 0, "adda.s", "suba.s", "mula.s", 0, "madd.s", "msub.s", "madda.s", "msuba.s",
    0, 0, 0, 0, "cvt.w.s", 0, 0, 0, "max.s", "min.s", 0, 0, 0, 0, 0, 0,
    "c.f.s", 0, "c.eq.s", 0, "c.lt.s", 0, "c.le.s", 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const char *const VU1[64] = {
    "vaddx", "vaddy", "vaddz", "vaddw", "vsubx", "vsuby", "vsubz", "vsubw", "vmaddx", "vmaddy", "vmaddz",
    "vmaddw", "vmsubx", "vmsuby", "vmsubz", "vmsubw", "vmaxx", "vmaxy", "vmaxz", "vmaxw", "vminix", "vminiy",
    "vminiz", "vminiw", "vmulx", "vmuly", "vmulz", "vmulw", "vmulq", "vmaxi", "vmuli", "vminii", "vaddq",
    "vmaddq", "vaddi", "vmaddi", "vsubq", "vmsubq", "vsubi", "vmsubi", "vadd", "vmadd", "vmul", "vmax",
    "vsub", "vmsub", "vopmsub", "vmini", "viadd", "visub", "viaddi", 0, "viand", "vior", 0, 0,
    "vcallms", "vcallmsr", 0, 0, 0, 0, 0, 0};

static const char *const VU2[0x44] = {
    "vaddax", "vadday", "vaddaz", "vaddaw", "vsubax", "vsubay", "vsubaz", "vsubaw", "vmaddax", "vmadday",
    "vmaddaz", "vmaddaw", "vmsubax", "vmsubay", "vmsubaz", "vmsubaw", "vitof0", "vitof4", "vitof12",
    "vitof15", "vftoi0", "vftoi4", "vftoi12", "vftoi15", "vmulax", "vmulay", "vmulaz", "vmulaw", "vmulaq",
    "vabs", "vmulai", "vclipw", "vaddaq", "vmaddaq", "vaddai", "vmaddai", "vsubaq", "vmsubaq", "vsubai",
    "vmsubai", "vadda", "vmadda", "vmula", 0, "vsuba", "vmsuba", "vopmula", "vnop", "vmove", "vmr32", 0, 0,
    "vlqi", "vsqi", "vlqd", "vsqd", "vdiv", "vsqrt", "vrsqrt", "vwaitq", "vmtir", "vmfir", "vilwr",
    "viswr", "vrnext", "vrget", "vrinit", "vrxor"};

static const char *dest(uint32_t w, char *b) {
    int n = 0;
    b[n++] = '.';
    if (w & (1u << 24)) b[n++] = 'x';
    if (w & (1u << 23)) b[n++] = 'y';
    if (w & (1u << 22)) b[n++] = 'z';
    if (w & (1u << 21)) b[n++] = 'w';
    b[n] = 0;
    return b;
}

const char *grx_mips_disasm(uint32_t w, uint32_t pc, char *buf, size_t n) {
    uint32_t op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, sa = (w >> 6) & 31;
    int16_t imm = (int16_t)w;
    const char *m;
    char d[8];
    switch (op) {
    case 0x00:
        m = SPECIAL[w & 63];
        if (!m) break;
        if (w == 0) {
            snprintf(buf, n, "nop");
        } else if ((w & 63) < 4 || (w & 63) >= 0x38) {
            snprintf(buf, n, "%s %s, %s, %u", m, R[rd], R[rt], sa);
        } else if ((w & 63) == 8) {
            snprintf(buf, n, "jr %s", R[rs]);
        } else if ((w & 63) == 9) {
            snprintf(buf, n, "jalr %s, %s", R[rd], R[rs]);
        } else {
            snprintf(buf, n, "%s %s, %s, %s", m, R[rd], R[rs], R[rt]);
        }
        return buf;
    case 0x01: {
        static const char *const RI[32] = {"bltz", "bgez", "bltzl", "bgezl", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           "bltzal", "bgezal", "bltzall", "bgezall"};
        if (!RI[rt]) break;
        snprintf(buf, n, "%s %s, 0x%08X", RI[rt], R[rs], pc + 4 + (uint32_t)(imm << 2));
        return buf;
    }
    case 0x02: case 0x03:
        snprintf(buf, n, "%s 0x%08X", OPS[op], ((pc + 4) & 0xF0000000u) | (w & 0x03FFFFFFu) << 2);
        return buf;
    case 0x04: case 0x05: case 0x14: case 0x15:
        snprintf(buf, n, "%s %s, %s, 0x%08X", OPS[op], R[rs], R[rt], pc + 4 + (uint32_t)(imm << 2));
        return buf;
    case 0x06: case 0x07: case 0x16: case 0x17:
        snprintf(buf, n, "%s %s, 0x%08X", OPS[op], R[rs], pc + 4 + (uint32_t)(imm << 2));
        return buf;
    case 0x0F:
        snprintf(buf, n, "lui %s, 0x%04X", R[rt], w & 0xFFFF);
        return buf;
    case 0x0C: case 0x0D: case 0x0E:
        snprintf(buf, n, "%s %s, %s, 0x%04X", OPS[op], R[rt], R[rs], w & 0xFFFF);
        return buf;
    case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x18: case 0x19:
        snprintf(buf, n, "%s %s, %s, %d", OPS[op], R[rt], R[rs], imm);
        return buf;
    case 0x11:
        if (rs == 0 || rs == 4) {
            snprintf(buf, n, "%s %s, $f%u", rs ? "mtc1" : "mfc1", R[rt], rd);
            return buf;
        }
        if (rs == 8) {
            static const char *const BC[4] = {"bc1f", "bc1t", "bc1fl", "bc1tl"};
            if (rt < 4) {
                snprintf(buf, n, "%s 0x%08X", BC[rt], pc + 4 + (uint32_t)(imm << 2));
                return buf;
            }
        }
        if (rs == 0x10 && FPU_S[w & 63]) {
            snprintf(buf, n, "%s $f%u, $f%u, $f%u", FPU_S[w & 63], sa, rd, rt);
            return buf;
        }
        if (rs == 0x14 && (w & 63) == 0x20) {
            snprintf(buf, n, "cvt.s.w $f%u, $f%u", sa, rd);
            return buf;
        }
        snprintf(buf, n, "cop1 rs=%02X fn=%02X", rs, w & 63);
        return buf;
    case 0x12:
        if (rs < 0x10) {
            static const char *const C2[16] = {0, "qmfc2", "cfc2", 0, 0, "qmtc2", "ctc2", 0, "bc2"};
            snprintf(buf, n, "%s %s, $vf%u", C2[rs] ? C2[rs] : "cop2?", R[rt], rd);
            return buf;
        }
        if ((w & 63) < 0x3C) {
            m = VU1[w & 63];
            snprintf(buf, n, "%s%s $vf%u, $vf%u, $vf%u", m ? m : "vu1?", dest(w, d), sa, rd, rt);
        } else {
            uint32_t k = sa << 2 | (w & 3);
            m = k < 0x44 ? VU2[k] : 0;
            snprintf(buf, n, "%s%s $vf%u, $vf%u", m ? m : "vu2?", dest(w, d), rt, rd);
        }
        return buf;
    case 0x1C: {
        static const char *const MMI[64] = {"madd", "maddu", 0, 0, "plzcw", 0, 0, 0, "mmi0", "mmi2", 0, 0, 0, 0, 0,
                                            0, "mfhi1", "mthi1", "mflo1", "mtlo1", 0, 0, 0, 0, "mult1", "multu1",
                                            "div1", "divu1", 0, 0, 0, 0, "madd1", "maddu1", 0, 0, 0, 0, 0, 0,
                                            "mmi1", "mmi3"};
        m = MMI[w & 63];
        if (!m) break;
        snprintf(buf, n, "%s %s, %s, %s (sub %02X)", m, R[rd], R[rs], R[rt], sa);
        return buf;
    }
    case 0x31: case 0x39:
        snprintf(buf, n, "%s $f%u, %d(%s)", OPS[op], rt, imm, R[rs]);
        return buf;
    case 0x36: case 0x3E:
        snprintf(buf, n, "%s $vf%u, %d(%s)", OPS[op], rt, imm, R[rs]);
        return buf;
    default:
        if (OPS[op]) {
            snprintf(buf, n, "%s %s, %d(%s)", OPS[op], R[rt], imm, R[rs]);
            return buf;
        }
    }
    snprintf(buf, n, "op=%02X rs=%u rt=%u rd=%u sa=%u fn=%02X", op, rs, rt, rd, sa, w & 63);
    return buf;
}
