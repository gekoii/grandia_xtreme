/* The game's VU0 microcode (vuMicroCode0) in C: the 24 kernels the CPU
   starts with vcallms (port/include/grx_vu0_micro.h, port/docs/vu.md).

   The kernels are straight-line: most are halves of a software pipeline
   across calls, finishing the previous vertex while starting the next, so
   which register holds what between calls matters to the callers. They are
   transcribed here pair by pair from the disassembly
   (build/port/vu/vuMicroCode0.txt, port/tools/vu_disasm.py), one call per
   operation, onto a small runtime that keeps the timing the code relies on:

   - Both operations of a pair read the registers as they were before the
     pair (results are committed at NEXT()); a later pair sees them (the VU
     stalls on VF register hazards, so this is the hardware's order).
   - Q from DIV is seen 7 pairs after the DIV issues; until then a read gets
     the previous Q. A DIV in a kernel's last pair is done by the next call.
   - The MAC and status flags of an FMAC operation, and the clip flag of a
     CLIP, are seen by FMAND/FCGET 4 pairs after it issues. FCGET in the
     billboard kernels relies on it: each picks up one vertex's two clip
     judgements out of eight CLIPs in a row.
   - MAX, MINI, FTOI and CLIP leave the MAC flags alone.
   Floats have no infinities or denormals (results saturate to +-FLT_MAX,
   tiny ones flush to zero); FTOI truncates and saturates. The VU's
   truncating rounding isn't modelled.

   A kernel runs to its end inside grx_vu0_callms(). On the PS2 the CPU goes
   on while it runs; the callers only touch registers the kernel has
   finished with (lqc2 into the other half's inputs) or wait for it
   (ctc2.i/qmfc2.i up to the M bit, or the next vcallms), so the results
   are the same. */
#include <stdint.h>
#include <string.h>
#include "grx_ee.h"
#include "grx_hwmem.h"
#include "grx_platform.h"
#include "grx_vu0_micro.h"

#pragma STDC FP_CONTRACT OFF

#define ACC GRX_ACC

/* VI16-VI27 as cfc2/ctc2 number them */
enum { VI_STATUS = 16, VI_MAC = 17, VI_CLIP = 18, VI_I = 21, VI_Q = 22, VI_CMSAR0 = 27 };

/* ---------------------------------------------------------------------- */
/* VU0 data memory: 4 KB, the mapped window at 0x11004000 (or a buffer
   when the window isn't mapped, as in tests) */

#define VU0_DATA_ADDR 0x11004000u
static uint8_t vu0_buffer[0x1000] __attribute__((aligned(16)));
static uint8_t *vu0_data;

static uint8_t *vu0_qw(uint32_t qa) {
    if (vu0_data == NULL) {
        vu0_data = grx_hwmem_mapped(VU0_DATA_ADDR, 0x1000) ? (uint8_t *)(uintptr_t)VU0_DATA_ADDR : vu0_buffer;
    }
    return vu0_data + ((qa & 0xFF) << 4);
}

/* ---------------------------------------------------------------------- */
/* Floats */

static float vu_fix(float x) {
    uint32_t b;
    memcpy(&b, &x, 4);
    if ((b & 0x7F800000u) == 0x7F800000u) {
        b = (b & 0x80000000u) | 0x7F7FFFFFu;
    } else if ((b & 0x7F800000u) == 0) {
        b &= 0x80000000u;
    }
    memcpy(&x, &b, 4);
    return x;
}

static float bits_f(uint32_t b) {
    float f;
    memcpy(&f, &b, 4);
    return f;
}

static uint32_t f_bits(float f) {
    uint32_t b;
    memcpy(&b, &f, 4);
    return b;
}

/* ---------------------------------------------------------------------- */
/* The runtime: one pair at a time */

static int cyc; /* pairs since the kernel started */

/* writes staged until the end of the pair */
static struct {
    int reg, mask;
    GrxQuad v;
} vf_w[2];
static int n_vf_w;
static struct {
    int reg;
    uint32_t v;
} vi_w[2];
static int n_vi_w;
static int i_w;
static uint32_t i_v;

/* Q: a DIV in flight */
static int q_pending, q_ready;
static uint32_t q_value;

/* flag writes in flight, in issue order: (cycle, value) */
#define MAXEV 64
static struct {
    int at;
    uint32_t mac, status;
} mac_ev[MAXEV];
static int n_mac;
static struct {
    int at;
    uint32_t clip;
} clip_ev[MAXEV];
static int n_clip;
static uint32_t mac0, status0, clip0, clip_issued, sticky;

static void q_settle(void) {
    if (q_pending && cyc >= q_ready) {
        grx_vi[VI_Q] = q_value;
        q_pending = 0;
    }
}

static float q_read(void) {
    q_settle();
    return bits_f(grx_vi[VI_Q]);
}

static uint32_t mac_seen(void) {
    int k;
    for (k = n_mac - 1; k >= 0; k--) {
        if (mac_ev[k].at <= cyc - 4) {
            return mac_ev[k].mac;
        }
    }
    return mac0;
}

static uint32_t clip_seen(void) {
    int k;
    for (k = n_clip - 1; k >= 0; k--) {
        if (clip_ev[k].at <= cyc - 4) {
            return clip_ev[k].clip;
        }
    }
    return clip0;
}

static void begin(void) {
    cyc = 0;
    q_ready = 0;
    q_settle(); /* a DIV left running by the last kernel is done by now */
    n_vf_w = n_vi_w = i_w = 0;
    n_mac = n_clip = 0;
    mac0 = grx_vi[VI_MAC];
    status0 = grx_vi[VI_STATUS];
    sticky = status0 & 0xFC0;
    clip0 = clip_issued = grx_vi[VI_CLIP] & 0xFFFFFF;
}

static void finish(void) {
    q_ready = 0;
    if (q_pending) {
        grx_vi[VI_Q] = q_value;
        q_pending = 0;
    }
    if (n_mac) {
        grx_vi[VI_MAC] = mac_ev[n_mac - 1].mac;
        grx_vi[VI_STATUS] = mac_ev[n_mac - 1].status;
    }
    if (n_clip) {
        grx_vi[VI_CLIP] = clip_ev[n_clip - 1].clip;
    }
}

static void stage_vf(int reg, int mask, const GrxQuad *v) {
    vf_w[n_vf_w].reg = reg;
    vf_w[n_vf_w].mask = mask;
    vf_w[n_vf_w].v = *v;
    n_vf_w++;
}

static void stage_vi(int reg, uint32_t v) {
    vi_w[n_vi_w].reg = reg;
    vi_w[n_vi_w].v = v & 0xFFFF;
    n_vi_w++;
}

static void NEXT(void) {
    int k, i;
    for (k = 0; k < n_vf_w; k++) {
        if (vf_w[k].reg == 0) {
            continue; /* vf0 is constant */
        }
        for (i = 0; i < 4; i++) {
            if (vf_w[k].mask & (1 << i)) {
                grx_vf[vf_w[k].reg].u[i] = vf_w[k].v.u[i];
            }
        }
    }
    for (k = 0; k < n_vi_w; k++) {
        if (vi_w[k].reg != 0) {
            grx_vi[vi_w[k].reg] = vi_w[k].v;
        }
    }
    if (i_w) {
        grx_vi[VI_I] = i_v;
    }
    n_vf_w = n_vi_w = i_w = 0;
    cyc++;
}

/* The MAC flags of a result (x: Z bit 3, S bit 7 ... w: Z bit 0, S bit 4)
   and the status flags they give */
static void flags(int mask, const GrxQuad *r) {
    uint32_t mac = 0, st;
    int i;
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            if (r->f[i] == 0.0f) {
                mac |= 1u << (3 - i);
            }
            if (r->u[i] & 0x80000000u) {
                mac |= 1u << (7 - i);
            }
        }
    }
    st = ((mac & 0xF) ? 1u : 0u) | ((mac & 0xF0) ? 2u : 0u);
    sticky |= st << 6;
    if (n_mac < MAXEV) {
        mac_ev[n_mac].at = cyc;
        mac_ev[n_mac].mac = mac;
        mac_ev[n_mac].status = st | sticky;
        n_mac++;
    }
}

/* ---------------------------------------------------------------------- */
/* Upper (FMAC) operations */

enum { OP_ADD, OP_SUB, OP_MUL, OP_MADD, OP_MSUB, OP_MAX, OP_MINI };
enum { BC_VEC = -1, BC_Q = -2, BC_I = -3 };

/* d = s op t; t broadcast from field bc, or the vector, Q or I. d may be
   ACC; MADD/MSUB take ACC as the first term. */
static void FMAC(int op, int mask, int d, int s, int t, int bc) {
    const float *a = grx_vf[s].f, *b = grx_vf[t].f, *acc = grx_vf[ACC].f;
    float q = bc == BC_Q ? q_read() : 0.0f, imm = bits_f(grx_vi[VI_I]);
    GrxQuad r;
    int i;

    memset(&r, 0, sizeof r);
    for (i = 0; i < 4; i++) {
        float y = bc >= 0 ? b[bc] : bc == BC_VEC ? b[i] : bc == BC_Q ? q : imm;
        float x;
        if (!(mask & (1 << i))) {
            continue;
        }
        switch (op) {
        case OP_ADD: x = a[i] + y; break;
        case OP_SUB: x = a[i] - y; break;
        case OP_MUL: x = a[i] * y; break;
        case OP_MADD: x = acc[i] + vu_fix(a[i] * y); break;
        case OP_MSUB: x = acc[i] - vu_fix(a[i] * y); break;
        case OP_MAX: x = a[i] > y ? a[i] : y; break;
        default: x = a[i] < y ? a[i] : y; break;
        }
        r.f[i] = vu_fix(x);
    }
    stage_vf(d, mask, &r);
    if (op != OP_MAX && op != OP_MINI) {
        flags(mask, &r);
    }
}

/* Outer product: ACC.xyz = s.yzx t.zxy; OPMSUB d.xyz = ACC - s.yzx t.zxy */
static void OPMULA(int s, int t) {
    const float *a = grx_vf[s].f, *b = grx_vf[t].f;
    GrxQuad r;
    memset(&r, 0, sizeof r);
    r.f[0] = vu_fix(a[1] * b[2]);
    r.f[1] = vu_fix(a[2] * b[0]);
    r.f[2] = vu_fix(a[0] * b[1]);
    stage_vf(ACC, 7, &r);
    flags(7, &r);
}

static void OPMSUB(int d, int s, int t) {
    const float *a = grx_vf[s].f, *b = grx_vf[t].f, *acc = grx_vf[ACC].f;
    GrxQuad r;
    memset(&r, 0, sizeof r);
    r.f[0] = vu_fix(acc[0] - vu_fix(a[1] * b[2]));
    r.f[1] = vu_fix(acc[1] - vu_fix(a[2] * b[0]));
    r.f[2] = vu_fix(acc[2] - vu_fix(a[0] * b[1]));
    stage_vf(d, 7, &r);
    flags(7, &r);
}

/* CLIPw.xyz s, t.w: +x -x +y -y +z -z against |t.w| into the low 6 bits
   of the clip flag, the older judgements shifting up */
static void CLIP(int s, int t) {
    const float *a = grx_vf[s].f;
    float w = grx_vf[t].f[3];
    uint32_t j = 0;
    if (w < 0.0f) {
        w = -w;
    }
    j |= a[0] > w ? 0x01u : 0;
    j |= a[0] < -w ? 0x02u : 0;
    j |= a[1] > w ? 0x04u : 0;
    j |= a[1] < -w ? 0x08u : 0;
    j |= a[2] > w ? 0x10u : 0;
    j |= a[2] < -w ? 0x20u : 0;
    clip_issued = ((clip_issued << 6) | j) & 0xFFFFFF;
    if (n_clip < MAXEV) {
        clip_ev[n_clip].at = cyc;
        clip_ev[n_clip].clip = clip_issued;
        n_clip++;
    }
}

static void FTOI(int mask, int d, int s, int frac) {
    GrxQuad r;
    int i;
    for (i = 0; i < 4; i++) {
        double x = (double)grx_vf[s].f[i] * (double)(1 << frac);
        r.i[i] = x != x ? 0 : x >= 2147483647.0 ? 0x7FFFFFFF : x <= -2147483648.0 ? (int)0x80000000u : (int)x;
    }
    stage_vf(d, mask, &r);
}

/* ---------------------------------------------------------------------- */
/* Lower operations */

static void MOVE(int mask, int d, int s) {
    stage_vf(d, mask, &grx_vf[s]);
}

static void MR32(int mask, int d, int s) {
    GrxQuad r;
    r.u[0] = grx_vf[s].u[1];
    r.u[1] = grx_vf[s].u[2];
    r.u[2] = grx_vf[s].u[3];
    r.u[3] = grx_vf[s].u[0];
    stage_vf(d, mask, &r);
}

/* DIV Q, s.sf / t.tf; x / 0 gives +-FLT_MAX */
static void DIV(int s, int sf, int t, int tf) {
    float a = grx_vf[s].f[sf], b = grx_vf[t].f[tf], q;
    if (b == 0.0f) {
        uint32_t sign = (f_bits(a) ^ f_bits(b)) & 0x80000000u;
        q = bits_f(a == 0.0f ? 0x7F7FFFFFu : sign | 0x7F7FFFFFu);
    } else {
        q = vu_fix(a / b);
    }
    q_settle();
    q_pending = 1;
    q_ready = cyc + 7;
    q_value = f_bits(q);
}

/* iaddiu and iaddi (the disassembly gives the immediate) */
static void IADDI(int t, int s, int imm) {
    stage_vi(t, grx_vi[s] + (uint32_t)imm);
}

static void ISUB(int d, int s, int t) {
    stage_vi(d, grx_vi[s] - grx_vi[t]);
}

static void IAND(int d, int s, int t) {
    stage_vi(d, grx_vi[s] & grx_vi[t]);
}

static void IOR(int d, int s, int t) {
    stage_vi(d, grx_vi[s] | grx_vi[t]);
}

/* MFIR: a VI register, sign-extended, into the fields of a VF register */
static void MFIR(int mask, int t, int s) {
    GrxQuad r;
    int i;
    for (i = 0; i < 4; i++) {
        r.i[i] = (int16_t)(grx_vi[s] & 0xFFFF);
    }
    stage_vf(t, mask, &r);
}

static void MTIR(int t, int s, int f) {
    stage_vi(t, grx_vf[s].u[f]);
}

static void SQI(int mask, int s, int t) {
    uint8_t *p = vu0_qw(grx_vi[t]);
    int i;
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            memcpy(p + 4 * i, &grx_vf[s].u[i], 4);
        }
    }
    stage_vi(t, grx_vi[t] + 1);
}

static void LQ(int mask, int t, int imm, int s) {
    GrxQuad r;
    memcpy(&r, vu0_qw(grx_vi[s] + (uint32_t)imm), 16);
    stage_vf(t, mask, &r);
}

static void FCGET(int t) {
    stage_vi(t, clip_seen() & 0xFFF);
}

static void FMAND(int t, int s) {
    stage_vi(t, grx_vi[s] & mac_seen());
}

static int IBNE(int s, int t) {
    return (grx_vi[s] & 0xFFFF) != (grx_vi[t] & 0xFFFF);
}

static void LOI(uint32_t bits) {
    i_w = 1;
    i_v = bits;
}

/* ---------------------------------------------------------------------- */
/* The kernels, from vu_disasm.py's listing (entry, then each pair to the
   one after the E bit). What each computes, and its caller:

   0x000, 0x080  VuMtxMulListMicro: vf08-vf11 = mtx(vf04-vf07) x vf08-vf11,
                 vf16-vf19 = mtx(vf12-vf15) x vf16-vf19 (column by column)
   0x100, 0x140, 0x1C0  VuProjectVerts: transform by vf04-vf07, q = vf18.w / z
                 (DIV, read by the next call), screen = vf17 + vf16 (x, y, 1) q
   0x240, 0x2A0  VuLightSelectNearest4 (uncalled): transposes 8 light
                 positions, squared distances from vf01 into vf16/vf17,
                 scaled by vf01.w^2, or 1.001 when vf01.w truncates to 0
   0x340         GphMeshClipTest: bounding box vf08-vf11 against the frustum;
                 vi02/vi03 = the sign bits of two differences (FMAND 0xF0)
   0x3C8, 0x508  GphVertXformClip: 4 + 4 vertices transformed (into
                 vf24-vf27 / vf28-vf31), projected, CLIP against w = 1 and
                 the guard band (vf02.w); the previous half's clip codes go
                 to the w words and to VU0 memory (SQI at vi03)
   0x648, 0x6C8  GphVertXform: the same transform without the codes
   0x748, 0x890  GphVertXformClipBillboard: a centre expanded into 4 corners
                 by the roll matrix vf08/vf09, projected and clip-coded
   0x9D8, 0xA40  GphVertXformBillboard: the corners, no projection
   0xAA8, 0xB08  GphTriCullClip(Near), GphNearClipTris: (b - a) x (c - a) . a
                 of one triangle; vi02/vi03 = its sign (FMAND 0x80)
   0xB68, 0xC18  GphTriCull: the same for 2 + 2 triangles whose vertices
                 come from VU0 memory by index (vf25-vf28 x/y/z & 0x7F)
   0xCC8, 0xD68  VuMeshProjectTexUV: vertices times the projector matrix
   0xE08, 0xE80  VuMeshProjectTexCull: per triangle, the AND of three clip
                 codes and the facing flag (vi11/vi12) */

static void k_0000(void) {
    FMAC(OP_MUL, 15, ACC, 4, 8, 0); NEXT(); /* 0000 mulax.xyzw ACC, vf04, vf08x */
    FMAC(OP_MADD, 15, ACC, 5, 8, 1); NEXT(); /* 0008 madday.xyzw ACC, vf05, vf08y */
    FMAC(OP_MADD, 15, ACC, 6, 8, 2); NEXT(); /* 0010 maddaz.xyzw ACC, vf06, vf08z */
    FMAC(OP_MADD, 15, 8, 7, 8, 3); NEXT(); /* 0018 maddw.xyzw vf08, vf07, vf08w */
    FMAC(OP_MUL, 15, ACC, 4, 9, 0); NEXT(); /* 0020 mulax.xyzw ACC, vf04, vf09x */
    FMAC(OP_MADD, 15, ACC, 5, 9, 1); NEXT(); /* 0028 madday.xyzw ACC, vf05, vf09y */
    FMAC(OP_MADD, 15, ACC, 6, 9, 2); NEXT(); /* 0030 maddaz.xyzw ACC, vf06, vf09z */
    FMAC(OP_MADD, 15, 9, 7, 9, 3); NEXT(); /* 0038 maddw.xyzw vf09, vf07, vf09w */
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); NEXT(); /* 0040 mulax.xyzw ACC, vf04, vf10x */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); NEXT(); /* 0048 madday.xyzw ACC, vf05, vf10y */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); NEXT(); /* 0050 maddaz.xyzw ACC, vf06, vf10z */
    FMAC(OP_MADD, 15, 10, 7, 10, 3); NEXT(); /* 0058 maddw.xyzw vf10, vf07, vf10w */
    FMAC(OP_MUL, 15, ACC, 4, 11, 0); NEXT(); /* 0060 mulax.xyzw ACC, vf04, vf11x */
    FMAC(OP_MADD, 15, ACC, 5, 11, 1); NEXT(); /* 0068 madday.xyzw ACC, vf05, vf11y */
    FMAC(OP_MADD, 15, ACC, 6, 11, 2); NEXT(); /* 0070 [E] maddaz.xyzw ACC, vf06, vf11z */
    FMAC(OP_MADD, 15, 11, 7, 11, 3); NEXT(); /* 0078 maddw.xyzw vf11, vf07, vf11w */
}

static void k_0080(void) {
    FMAC(OP_MUL, 15, ACC, 12, 16, 0); NEXT(); /* 0080 mulax.xyzw ACC, vf12, vf16x */
    FMAC(OP_MADD, 15, ACC, 13, 16, 1); NEXT(); /* 0088 madday.xyzw ACC, vf13, vf16y */
    FMAC(OP_MADD, 15, ACC, 14, 16, 2); NEXT(); /* 0090 maddaz.xyzw ACC, vf14, vf16z */
    FMAC(OP_MADD, 15, 16, 15, 16, 3); NEXT(); /* 0098 maddw.xyzw vf16, vf15, vf16w */
    FMAC(OP_MUL, 15, ACC, 12, 17, 0); NEXT(); /* 00a0 mulax.xyzw ACC, vf12, vf17x */
    FMAC(OP_MADD, 15, ACC, 13, 17, 1); NEXT(); /* 00a8 madday.xyzw ACC, vf13, vf17y */
    FMAC(OP_MADD, 15, ACC, 14, 17, 2); NEXT(); /* 00b0 maddaz.xyzw ACC, vf14, vf17z */
    FMAC(OP_MADD, 15, 17, 15, 17, 3); NEXT(); /* 00b8 maddw.xyzw vf17, vf15, vf17w */
    FMAC(OP_MUL, 15, ACC, 12, 18, 0); NEXT(); /* 00c0 mulax.xyzw ACC, vf12, vf18x */
    FMAC(OP_MADD, 15, ACC, 13, 18, 1); NEXT(); /* 00c8 madday.xyzw ACC, vf13, vf18y */
    FMAC(OP_MADD, 15, ACC, 14, 18, 2); NEXT(); /* 00d0 maddaz.xyzw ACC, vf14, vf18z */
    FMAC(OP_MADD, 15, 18, 15, 18, 3); NEXT(); /* 00d8 maddw.xyzw vf18, vf15, vf18w */
    FMAC(OP_MUL, 15, ACC, 12, 19, 0); NEXT(); /* 00e0 mulax.xyzw ACC, vf12, vf19x */
    FMAC(OP_MADD, 15, ACC, 13, 19, 1); NEXT(); /* 00e8 madday.xyzw ACC, vf13, vf19y */
    FMAC(OP_MADD, 15, ACC, 14, 19, 2); NEXT(); /* 00f0 [E] maddaz.xyzw ACC, vf14, vf19z */
    FMAC(OP_MADD, 15, 19, 15, 19, 3); NEXT(); /* 00f8 maddw.xyzw vf19, vf15, vf19w */
}

static void k_0100(void) {
    FMAC(OP_MUL, 15, ACC, 4, 8, 0); NEXT(); /* 0100 mulax.xyzw ACC, vf04, vf08x */
    FMAC(OP_MADD, 15, ACC, 5, 8, 1); NEXT(); /* 0108 madday.xyzw ACC, vf05, vf08y */
    FMAC(OP_MADD, 15, ACC, 6, 8, 2); NEXT(); /* 0110 maddaz.xyzw ACC, vf06, vf08z */
    FMAC(OP_MADD, 15, 8, 7, 0, 3); NEXT(); /* 0118 maddw.xyzw vf08, vf07, vf00w */
    NEXT(); /* 0120 nop */
    MOVE(8, 12, 0); NEXT(); /* 0128 move.w vf12, vf00 */
    MOVE(4, 12, 16); NEXT(); /* 0130 [E] move.z vf12, vf16 */
    FMAC(OP_MUL, 3, 12, 16, 8, BC_VEC); DIV(18, 3, 8, 2); NEXT(); /* 0138 mul.xy vf12, vf16, vf08 | div Q, vf18w, vf08z */
}

static void k_0140(void) {
    FMAC(OP_MUL, 15, ACC, 4, 9, 0); NEXT(); /* 0140 mulax.xyzw ACC, vf04, vf09x */
    FMAC(OP_MADD, 15, ACC, 5, 9, 1); NEXT(); /* 0148 madday.xyzw ACC, vf05, vf09y */
    FMAC(OP_MADD, 15, ACC, 6, 9, 2); NEXT(); /* 0150 maddaz.xyzw ACC, vf06, vf09z */
    FMAC(OP_MADD, 15, 9, 7, 0, 3); NEXT(); /* 0158 maddw.xyzw vf09, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 17, 0, 3); NEXT(); /* 0160 mulaw.xyzw ACC, vf17, vf00w */
    MOVE(8, 13, 0); NEXT(); /* 0168 move.w vf13, vf00 */
    FMAC(OP_MADD, 7, 12, 12, 0, BC_Q); MOVE(4, 13, 16); NEXT(); /* 0170 maddq.xyz vf12, vf12, Q | move.z vf13, vf16 */
    FMAC(OP_MUL, 3, 13, 16, 9, BC_VEC); DIV(18, 3, 9, 2); NEXT(); /* 0178 mul.xy vf13, vf16, vf09 | div Q, vf18w, vf09z */
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); NEXT(); /* 0180 mulax.xyzw ACC, vf04, vf10x */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); NEXT(); /* 0188 madday.xyzw ACC, vf05, vf10y */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); NEXT(); /* 0190 maddaz.xyzw ACC, vf06, vf10z */
    FMAC(OP_MADD, 15, 10, 7, 0, 3); NEXT(); /* 0198 maddw.xyzw vf10, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 17, 0, 3); NEXT(); /* 01a0 mulaw.xyzw ACC, vf17, vf00w */
    MOVE(8, 14, 0); NEXT(); /* 01a8 move.w vf14, vf00 */
    FMAC(OP_MADD, 7, 13, 13, 0, BC_Q); MOVE(4, 14, 16); NEXT(); /* 01b0 [E] maddq.xyz vf13, vf13, Q | move.z vf14, vf16 */
    FMAC(OP_MUL, 3, 14, 16, 10, BC_VEC); DIV(18, 3, 10, 2); NEXT(); /* 01b8 mul.xy vf14, vf16, vf10 | div Q, vf18w, vf10z */
}

static void k_01c0(void) {
    FMAC(OP_MUL, 15, ACC, 4, 11, 0); NEXT(); /* 01c0 mulax.xyzw ACC, vf04, vf11x */
    FMAC(OP_MADD, 15, ACC, 5, 11, 1); NEXT(); /* 01c8 madday.xyzw ACC, vf05, vf11y */
    FMAC(OP_MADD, 15, ACC, 6, 11, 2); NEXT(); /* 01d0 maddaz.xyzw ACC, vf06, vf11z */
    FMAC(OP_MADD, 15, 11, 7, 0, 3); NEXT(); /* 01d8 maddw.xyzw vf11, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 17, 0, 3); NEXT(); /* 01e0 mulaw.xyzw ACC, vf17, vf00w */
    MOVE(8, 15, 0); NEXT(); /* 01e8 move.w vf15, vf00 */
    FMAC(OP_MADD, 7, 14, 14, 0, BC_Q); MOVE(4, 15, 16); NEXT(); /* 01f0 maddq.xyz vf14, vf14, Q | move.z vf15, vf16 */
    FMAC(OP_MUL, 3, 15, 16, 11, BC_VEC); DIV(18, 3, 11, 2); NEXT(); /* 01f8 mul.xy vf15, vf16, vf11 | div Q, vf18w, vf11z */
    FMAC(OP_MUL, 15, ACC, 4, 8, 0); NEXT(); /* 0200 mulax.xyzw ACC, vf04, vf08x */
    FMAC(OP_MADD, 15, ACC, 5, 8, 1); NEXT(); /* 0208 madday.xyzw ACC, vf05, vf08y */
    FMAC(OP_MADD, 15, ACC, 6, 8, 2); NEXT(); /* 0210 maddaz.xyzw ACC, vf06, vf08z */
    FMAC(OP_MADD, 15, 8, 7, 0, 3); NEXT(); /* 0218 maddw.xyzw vf08, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 17, 0, 3); NEXT(); /* 0220 mulaw.xyzw ACC, vf17, vf00w */
    MOVE(8, 12, 0); NEXT(); /* 0228 move.w vf12, vf00 */
    FMAC(OP_MADD, 7, 15, 15, 0, BC_Q); MOVE(4, 12, 16); NEXT(); /* 0230 [E] maddq.xyz vf15, vf15, Q | move.z vf12, vf16 */
    FMAC(OP_MUL, 3, 12, 16, 8, BC_VEC); DIV(18, 3, 8, 2); NEXT(); /* 0238 mul.xy vf12, vf16, vf08 | div Q, vf18w, vf08z */
}

static void k_0240(void) {
    int br;
    FMAC(OP_ADD, 1, 4, 0, 8, 2); MOVE(1, 2, 8); NEXT(); /* 0240 addz.x vf04, vf00, vf08z | move.x vf02, vf08 */
    FMAC(OP_ADD, 2, 2, 0, 9, 0); MR32(1, 3, 8); NEXT(); /* 0248 addx.y vf02, vf00, vf09x | mr32.x vf03, vf08 */
    FMAC(OP_ADD, 4, 2, 0, 10, 0); MOVE(2, 3, 9); NEXT(); /* 0250 addx.z vf02, vf00, vf10x | move.y vf03, vf09 */
    FMAC(OP_ADD, 4, 3, 0, 10, 1); MR32(2, 4, 9); NEXT(); /* 0258 addy.z vf03, vf00, vf10y | mr32.y vf04, vf09 */
    FMAC(OP_MUL, 8, 3, 0, 11, 1); MOVE(4, 4, 10); NEXT(); /* 0260 muly.w vf03, vf00, vf11y | move.z vf04, vf10 */
    FMAC(OP_MUL, 8, 4, 0, 11, 2); MR32(8, 2, 11); NEXT(); /* 0268 mulz.w vf04, vf00, vf11z | mr32.w vf02, vf11 */
    FMAC(OP_ADD, 1, 7, 0, 12, 2); MOVE(1, 5, 12); NEXT(); /* 0270 addz.x vf07, vf00, vf12z | move.x vf05, vf12 */
    FMAC(OP_ADD, 2, 5, 0, 13, 0); MR32(1, 6, 12); NEXT(); /* 0278 addx.y vf05, vf00, vf13x | mr32.x vf06, vf12 */
    FMAC(OP_ADD, 4, 5, 0, 14, 0); MOVE(2, 6, 13); NEXT(); /* 0280 addx.z vf05, vf00, vf14x | move.y vf06, vf13 */
    FMAC(OP_ADD, 4, 6, 0, 14, 1); MR32(2, 7, 13); NEXT(); /* 0288 addy.z vf06, vf00, vf14y | mr32.y vf07, vf13 */
    FMAC(OP_MUL, 8, 6, 0, 15, 1); MOVE(4, 7, 14); NEXT(); /* 0290 muly.w vf06, vf00, vf15y | move.z vf07, vf14 */
    FMAC(OP_MUL, 8, 7, 0, 15, 2); MR32(8, 5, 15); NEXT(); /* 0298 mulz.w vf07, vf00, vf15z | mr32.w vf05, vf15 */
    FMAC(OP_MUL, 8, 1, 1, 1, BC_VEC); NEXT(); /* 02a0 mul.w vf01, vf01, vf01 */
    FMAC(OP_SUB, 15, 8, 2, 1, 0); NEXT(); /* 02a8 subx.xyzw vf08, vf02, vf01x */
    FMAC(OP_SUB, 15, 9, 3, 1, 1); NEXT(); /* 02b0 suby.xyzw vf09, vf03, vf01y */
    FMAC(OP_SUB, 15, 10, 4, 1, 2); NEXT(); /* 02b8 subz.xyzw vf10, vf04, vf01z */
    FTOI(8, 18, 1, 0); NEXT(); /* 02c0 ftoi0.w vf18, vf01 */
    FMAC(OP_SUB, 15, 12, 5, 1, 0); NEXT(); /* 02c8 subx.xyzw vf12, vf05, vf01x */
    FMAC(OP_SUB, 15, 13, 6, 1, 1); NEXT(); /* 02d0 suby.xyzw vf13, vf06, vf01y */
    FMAC(OP_SUB, 15, 14, 7, 1, 2); NEXT(); /* 02d8 subz.xyzw vf14, vf07, vf01z */
    FMAC(OP_MUL, 15, ACC, 8, 8, BC_VEC); MTIR(4, 18, 3); NEXT(); /* 02e0 mula.xyzw ACC, vf08, vf08 | mtir vi04, vf18w */
    FMAC(OP_MADD, 15, ACC, 9, 9, BC_VEC); NEXT(); /* 02e8 madda.xyzw ACC, vf09, vf09 */
    FMAC(OP_MADD, 15, 16, 10, 10, BC_VEC); NEXT(); /* 02f0 madd.xyzw vf16, vf10, vf10 */
    FMAC(OP_MUL, 15, ACC, 12, 12, BC_VEC); NEXT(); /* 02f8 mula.xyzw ACC, vf12, vf12 */
    FMAC(OP_MADD, 15, ACC, 13, 13, BC_VEC); br = IBNE(4, 0); NEXT(); /* 0300 madda.xyzw ACC, vf13, vf13 | ibne vi04, vi00, L_0320 */
    FMAC(OP_MADD, 15, 17, 14, 14, BC_VEC); NEXT(); /* 0308 madd.xyzw vf17, vf14, vf14 */
    if (br) {
        FMAC(OP_MUL, 15, 16, 0, 0, 0); LOI(0x3f8020c5); NEXT(); /* 0320 [I] mulx.xyzw vf16, vf00, vf00x | loi 1.00100005  (0x3f8020c5) */
        FMAC(OP_MUL, 15, 17, 0, 0, 0); NEXT(); /* 0328 mulx.xyzw vf17, vf00, vf00x */
        FMAC(OP_ADD, 15, 16, 16, 0, BC_I); NEXT(); /* 0330 [E] addi.xyzw vf16, vf16, I */
        FMAC(OP_ADD, 15, 17, 17, 0, BC_I); NEXT(); /* 0338 addi.xyzw vf17, vf17, I */
    } else {
        FMAC(OP_MUL, 15, 16, 16, 1, 3); NEXT(); /* 0310 [E] mulw.xyzw vf16, vf16, vf01w */
        FMAC(OP_MUL, 15, 17, 17, 1, 3); NEXT(); /* 0318 mulw.xyzw vf17, vf17, vf01w */
    }
}

static void k_02a0(void) {
    int br;
    FMAC(OP_MUL, 8, 1, 1, 1, BC_VEC); NEXT(); /* 02a0 mul.w vf01, vf01, vf01 */
    FMAC(OP_SUB, 15, 8, 2, 1, 0); NEXT(); /* 02a8 subx.xyzw vf08, vf02, vf01x */
    FMAC(OP_SUB, 15, 9, 3, 1, 1); NEXT(); /* 02b0 suby.xyzw vf09, vf03, vf01y */
    FMAC(OP_SUB, 15, 10, 4, 1, 2); NEXT(); /* 02b8 subz.xyzw vf10, vf04, vf01z */
    FTOI(8, 18, 1, 0); NEXT(); /* 02c0 ftoi0.w vf18, vf01 */
    FMAC(OP_SUB, 15, 12, 5, 1, 0); NEXT(); /* 02c8 subx.xyzw vf12, vf05, vf01x */
    FMAC(OP_SUB, 15, 13, 6, 1, 1); NEXT(); /* 02d0 suby.xyzw vf13, vf06, vf01y */
    FMAC(OP_SUB, 15, 14, 7, 1, 2); NEXT(); /* 02d8 subz.xyzw vf14, vf07, vf01z */
    FMAC(OP_MUL, 15, ACC, 8, 8, BC_VEC); MTIR(4, 18, 3); NEXT(); /* 02e0 mula.xyzw ACC, vf08, vf08 | mtir vi04, vf18w */
    FMAC(OP_MADD, 15, ACC, 9, 9, BC_VEC); NEXT(); /* 02e8 madda.xyzw ACC, vf09, vf09 */
    FMAC(OP_MADD, 15, 16, 10, 10, BC_VEC); NEXT(); /* 02f0 madd.xyzw vf16, vf10, vf10 */
    FMAC(OP_MUL, 15, ACC, 12, 12, BC_VEC); NEXT(); /* 02f8 mula.xyzw ACC, vf12, vf12 */
    FMAC(OP_MADD, 15, ACC, 13, 13, BC_VEC); br = IBNE(4, 0); NEXT(); /* 0300 madda.xyzw ACC, vf13, vf13 | ibne vi04, vi00, L_0320 */
    FMAC(OP_MADD, 15, 17, 14, 14, BC_VEC); NEXT(); /* 0308 madd.xyzw vf17, vf14, vf14 */
    if (br) {
        FMAC(OP_MUL, 15, 16, 0, 0, 0); LOI(0x3f8020c5); NEXT(); /* 0320 [I] mulx.xyzw vf16, vf00, vf00x | loi 1.00100005  (0x3f8020c5) */
        FMAC(OP_MUL, 15, 17, 0, 0, 0); NEXT(); /* 0328 mulx.xyzw vf17, vf00, vf00x */
        FMAC(OP_ADD, 15, 16, 16, 0, BC_I); NEXT(); /* 0330 [E] addi.xyzw vf16, vf16, I */
        FMAC(OP_ADD, 15, 17, 17, 0, BC_I); NEXT(); /* 0338 addi.xyzw vf17, vf17, I */
    } else {
        FMAC(OP_MUL, 15, 16, 16, 1, 3); NEXT(); /* 0310 [E] mulw.xyzw vf16, vf16, vf01w */
        FMAC(OP_MUL, 15, 17, 17, 1, 3); NEXT(); /* 0318 mulw.xyzw vf17, vf17, vf01w */
    }
}

static void k_0340(void) {
    FMAC(OP_MUL, 15, 1, 0, 0, 0); NEXT(); /* 0340 mulx.xyzw vf01, vf00, vf00x */
    FMAC(OP_MUL, 15, 10, 10, 9, 0); NEXT(); /* 0348 mulx.xyzw vf10, vf10, vf09x */
    FMAC(OP_ADD, 3, 1, 1, 8, 0); NEXT(); /* 0350 addx.xy vf01, vf01, vf08x */
    FMAC(OP_ADD, 12, 1, 1, 8, 1); NEXT(); /* 0358 addy.zw vf01, vf01, vf08y */
    FMAC(OP_MUL, 15, 11, 11, 8, 2); NEXT(); /* 0360 mulz.xyzw vf11, vf11, vf08z */
    FMAC(OP_SUB, 10, 13, 1, 10, BC_VEC); NEXT(); /* 0368 sub.yw vf13, vf01, vf10 */
    FMAC(OP_ADD, 5, 13, 1, 10, BC_VEC); NEXT(); /* 0370 add.xz vf13, vf01, vf10 */
    FMAC(OP_SUB, 5, 12, 1, 10, BC_VEC); NEXT(); /* 0378 sub.xz vf12, vf01, vf10 */
    FMAC(OP_ADD, 10, 12, 1, 10, BC_VEC); NEXT(); /* 0380 add.yw vf12, vf01, vf10 */
    FMAC(OP_SUB, 15, 14, 11, 13, BC_VEC); IADDI(1, 0, 240); NEXT(); /* 0388 sub.xyzw vf14, vf11, vf13 | iaddiu vi01, vi00, 0xf0 */
    FMAC(OP_SUB, 15, 14, 11, 12, BC_VEC); NEXT(); /* 0390 sub.xyzw vf14, vf11, vf12 */
    NEXT(); /* 0398 nop */
    NEXT(); /* 03a0 nop */
    FMAND(2, 1); NEXT(); /* 03a8 fmand vi02, vi01 */
    FMAND(3, 1); NEXT(); /* 03b0 fmand vi03, vi01 */
    NEXT(); /* 03b8 [E] nop */
    NEXT(); /* 03c0 nop */
}

static void k_03c8(void) {
    FMAC(OP_MUL, 15, ACC, 4, 8, 0); ISUB(4, 1, 2); NEXT(); /* 03c8 mulax.xyzw ACC, vf04, vf08x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 31, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 03d0 max.z vf03, vf31, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 8, 1); IAND(5, 2, 7); NEXT(); /* 03d8 madday.xyzw ACC, vf05, vf08y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 22, 22, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 03e0 mulq.xy vf22, vf22, Q | ior vi02, vi04, vi05 */
    CLIP(21, 0); MFIR(8, 28, 2); NEXT(); /* 03e8 clipw.xyz vf21, vf00w | mfir.w vf28, vi02 */
    CLIP(21, 2); DIV(1, 3, 3, 2); NEXT(); /* 03f0 clipw.xyz vf21, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 8, 2); MR32(4, 3, 3); NEXT(); /* 03f8 maddaz.xyzw ACC, vf06, vf08z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 24, 7, 0, 3); IADDI(1, 0, -1); NEXT(); /* 0400 maddw.xyzw vf24, vf07, vf00w | iaddi vi01, vi00, -0x1 */
    FMAC(OP_MUL, 7, 23, 31, 1, BC_VEC); SQI(15, 28, 3); NEXT(); /* 0408 mul.xyz vf23, vf31, vf01 | sqi.xyzw vf28, (vi03++) */
    FMAC(OP_ADD, 7, 22, 22, 2, BC_VEC); FCGET(2); NEXT(); /* 0410 add.xyz vf22, vf22, vf02 | fcget vi02 */
    FMAC(OP_MUL, 15, ACC, 4, 9, 0); ISUB(4, 1, 2); NEXT(); /* 0418 mulax.xyzw ACC, vf04, vf09x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 24, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0420 max.z vf03, vf24, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 9, 1); IAND(5, 2, 7); NEXT(); /* 0428 madday.xyzw ACC, vf05, vf09y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 23, 23, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 0430 mulq.xy vf23, vf23, Q | ior vi02, vi04, vi05 */
    CLIP(22, 0); MFIR(8, 29, 2); NEXT(); /* 0438 clipw.xyz vf22, vf00w | mfir.w vf29, vi02 */
    CLIP(22, 2); DIV(1, 3, 3, 2); NEXT(); /* 0440 clipw.xyz vf22, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 9, 2); MR32(4, 3, 3); NEXT(); /* 0448 maddaz.xyzw ACC, vf06, vf09z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 25, 7, 0, 3); IADDI(6, 0, 1008); NEXT(); /* 0450 maddw.xyzw vf25, vf07, vf00w | iaddiu vi06, vi00, 0x3f0 */
    FMAC(OP_MUL, 7, 16, 24, 1, BC_VEC); SQI(15, 29, 3); NEXT(); /* 0458 mul.xyz vf16, vf24, vf01 | sqi.xyzw vf29, (vi03++) */
    FMAC(OP_ADD, 7, 23, 23, 2, BC_VEC); FCGET(2); NEXT(); /* 0460 add.xyz vf23, vf23, vf02 | fcget vi02 */
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); ISUB(4, 1, 2); NEXT(); /* 0468 mulax.xyzw ACC, vf04, vf10x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 25, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0470 max.z vf03, vf25, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); IAND(5, 2, 7); NEXT(); /* 0478 madday.xyzw ACC, vf05, vf10y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 16, 16, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 0480 mulq.xy vf16, vf16, Q | ior vi02, vi04, vi05 */
    CLIP(23, 0); MFIR(8, 30, 2); NEXT(); /* 0488 clipw.xyz vf23, vf00w | mfir.w vf30, vi02 */
    CLIP(23, 2); DIV(1, 3, 3, 2); NEXT(); /* 0490 clipw.xyz vf23, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); MR32(4, 3, 3); NEXT(); /* 0498 maddaz.xyzw ACC, vf06, vf10z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 26, 7, 0, 3); IADDI(7, 0, 15); NEXT(); /* 04a0 maddw.xyzw vf26, vf07, vf00w | iaddiu vi07, vi00, 0xf */
    FMAC(OP_MUL, 7, 17, 25, 1, BC_VEC); SQI(15, 30, 3); NEXT(); /* 04a8 mul.xyz vf17, vf25, vf01 | sqi.xyzw vf30, (vi03++) */
    FMAC(OP_ADD, 7, 16, 16, 2, BC_VEC); FCGET(2); NEXT(); /* 04b0 add.xyz vf16, vf16, vf02 | fcget vi02 */
    FMAC(OP_MUL, 15, ACC, 4, 11, 0); ISUB(4, 1, 2); NEXT(); /* 04b8 mulax.xyzw ACC, vf04, vf11x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 26, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 04c0 max.z vf03, vf26, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 11, 1); IAND(5, 2, 7); NEXT(); /* 04c8 madday.xyzw ACC, vf05, vf11y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 17, 17, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 04d0 mulq.xy vf17, vf17, Q | ior vi02, vi04, vi05 */
    CLIP(16, 0); MFIR(8, 31, 2); NEXT(); /* 04d8 [M] clipw.xyz vf16, vf00w | mfir.w vf31, vi02 */
    CLIP(16, 2); DIV(1, 3, 3, 2); NEXT(); /* 04e0 [M] clipw.xyz vf16, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 11, 2); MR32(4, 3, 3); NEXT(); /* 04e8 [M] maddaz.xyzw ACC, vf06, vf11z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 27, 7, 0, 3); NEXT(); /* 04f0 [M] maddw.xyzw vf27, vf07, vf00w */
    FMAC(OP_MUL, 7, 18, 26, 1, BC_VEC); SQI(15, 31, 3); NEXT(); /* 04f8 [E] mul.xyz vf18, vf26, vf01 | sqi.xyzw vf31, (vi03++) */
    FMAC(OP_ADD, 7, 17, 17, 2, BC_VEC); FCGET(2); NEXT(); /* 0500 add.xyz vf17, vf17, vf02 | fcget vi02 */
}

static void k_0508(void) {
    FMAC(OP_MUL, 15, ACC, 4, 12, 0); ISUB(4, 1, 2); NEXT(); /* 0508 mulax.xyzw ACC, vf04, vf12x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 27, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0510 max.z vf03, vf27, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 12, 1); IAND(5, 2, 7); NEXT(); /* 0518 madday.xyzw ACC, vf05, vf12y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 18, 18, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 0520 mulq.xy vf18, vf18, Q | ior vi02, vi04, vi05 */
    CLIP(17, 0); MFIR(8, 24, 2); NEXT(); /* 0528 clipw.xyz vf17, vf00w | mfir.w vf24, vi02 */
    CLIP(17, 2); DIV(1, 3, 3, 2); NEXT(); /* 0530 clipw.xyz vf17, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 12, 2); MR32(4, 3, 3); NEXT(); /* 0538 maddaz.xyzw ACC, vf06, vf12z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 28, 7, 0, 3); IADDI(1, 0, -1); NEXT(); /* 0540 maddw.xyzw vf28, vf07, vf00w | iaddi vi01, vi00, -0x1 */
    FMAC(OP_MUL, 7, 19, 27, 1, BC_VEC); SQI(15, 24, 3); NEXT(); /* 0548 mul.xyz vf19, vf27, vf01 | sqi.xyzw vf24, (vi03++) */
    FMAC(OP_ADD, 7, 18, 18, 2, BC_VEC); FCGET(2); NEXT(); /* 0550 add.xyz vf18, vf18, vf02 | fcget vi02 */
    FMAC(OP_MUL, 15, ACC, 4, 13, 0); ISUB(4, 1, 2); NEXT(); /* 0558 mulax.xyzw ACC, vf04, vf13x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 28, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0560 max.z vf03, vf28, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 13, 1); IAND(5, 2, 7); NEXT(); /* 0568 madday.xyzw ACC, vf05, vf13y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 19, 19, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 0570 mulq.xy vf19, vf19, Q | ior vi02, vi04, vi05 */
    CLIP(18, 0); MFIR(8, 25, 2); NEXT(); /* 0578 clipw.xyz vf18, vf00w | mfir.w vf25, vi02 */
    CLIP(18, 2); DIV(1, 3, 3, 2); NEXT(); /* 0580 clipw.xyz vf18, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 13, 2); MR32(4, 3, 3); NEXT(); /* 0588 maddaz.xyzw ACC, vf06, vf13z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 29, 7, 0, 3); IADDI(6, 0, 1008); NEXT(); /* 0590 maddw.xyzw vf29, vf07, vf00w | iaddiu vi06, vi00, 0x3f0 */
    FMAC(OP_MUL, 7, 20, 28, 1, BC_VEC); SQI(15, 25, 3); NEXT(); /* 0598 mul.xyz vf20, vf28, vf01 | sqi.xyzw vf25, (vi03++) */
    FMAC(OP_ADD, 7, 19, 19, 2, BC_VEC); FCGET(2); NEXT(); /* 05a0 add.xyz vf19, vf19, vf02 | fcget vi02 */
    FMAC(OP_MUL, 15, ACC, 4, 14, 0); ISUB(4, 1, 2); NEXT(); /* 05a8 mulax.xyzw ACC, vf04, vf14x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 29, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 05b0 max.z vf03, vf29, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 14, 1); IAND(5, 2, 7); NEXT(); /* 05b8 madday.xyzw ACC, vf05, vf14y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 20, 20, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 05c0 mulq.xy vf20, vf20, Q | ior vi02, vi04, vi05 */
    CLIP(19, 0); MFIR(8, 26, 2); NEXT(); /* 05c8 clipw.xyz vf19, vf00w | mfir.w vf26, vi02 */
    CLIP(19, 2); DIV(1, 3, 3, 2); NEXT(); /* 05d0 clipw.xyz vf19, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 14, 2); MR32(4, 3, 3); NEXT(); /* 05d8 maddaz.xyzw ACC, vf06, vf14z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 30, 7, 0, 3); IADDI(7, 0, 15); NEXT(); /* 05e0 maddw.xyzw vf30, vf07, vf00w | iaddiu vi07, vi00, 0xf */
    FMAC(OP_MUL, 7, 21, 29, 1, BC_VEC); SQI(15, 26, 3); NEXT(); /* 05e8 mul.xyz vf21, vf29, vf01 | sqi.xyzw vf26, (vi03++) */
    FMAC(OP_ADD, 7, 20, 20, 2, BC_VEC); FCGET(2); NEXT(); /* 05f0 add.xyz vf20, vf20, vf02 | fcget vi02 */
    FMAC(OP_MUL, 15, ACC, 4, 15, 0); ISUB(4, 1, 2); NEXT(); /* 05f8 mulax.xyzw ACC, vf04, vf15x | isub vi04, vi01, vi02 */
    FMAC(OP_MAX, 4, 3, 30, 3, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0600 max.z vf03, vf30, vf03 | iand vi04, vi04, vi06 */
    FMAC(OP_MADD, 15, ACC, 5, 15, 1); IAND(5, 2, 7); NEXT(); /* 0608 madday.xyzw ACC, vf05, vf15y | iand vi05, vi02, vi07 */
    FMAC(OP_MUL, 3, 21, 21, 0, BC_Q); IOR(2, 4, 5); NEXT(); /* 0610 mulq.xy vf21, vf21, Q | ior vi02, vi04, vi05 */
    CLIP(20, 0); MFIR(8, 27, 2); NEXT(); /* 0618 [M] clipw.xyz vf20, vf00w | mfir.w vf27, vi02 */
    CLIP(20, 2); DIV(1, 3, 3, 2); NEXT(); /* 0620 [M] clipw.xyz vf20, vf02w | div Q, vf01w, vf03z */
    FMAC(OP_MADD, 15, ACC, 6, 15, 2); MR32(4, 3, 3); NEXT(); /* 0628 [M] maddaz.xyzw ACC, vf06, vf15z | mr32.z vf03, vf03 */
    FMAC(OP_MADD, 15, 31, 7, 0, 3); NEXT(); /* 0630 [M] maddw.xyzw vf31, vf07, vf00w */
    FMAC(OP_MUL, 7, 22, 30, 1, BC_VEC); SQI(15, 27, 3); NEXT(); /* 0638 [E] mul.xyz vf22, vf30, vf01 | sqi.xyzw vf27, (vi03++) */
    FMAC(OP_ADD, 7, 21, 21, 2, BC_VEC); FCGET(2); NEXT(); /* 0640 add.xyz vf21, vf21, vf02 | fcget vi02 */
}

static void k_0648(void) {
    FMAC(OP_MUL, 15, ACC, 4, 8, 0); SQI(15, 28, 3); NEXT(); /* 0648 mulax.xyzw ACC, vf04, vf08x | sqi.xyzw vf28, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 8, 1); NEXT(); /* 0650 madday.xyzw ACC, vf05, vf08y */
    FMAC(OP_MADD, 15, ACC, 6, 8, 2); NEXT(); /* 0658 maddaz.xyzw ACC, vf06, vf08z */
    FMAC(OP_MADD, 15, 24, 7, 0, 3); NEXT(); /* 0660 maddw.xyzw vf24, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 9, 0); SQI(15, 29, 3); NEXT(); /* 0668 mulax.xyzw ACC, vf04, vf09x | sqi.xyzw vf29, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 9, 1); NEXT(); /* 0670 madday.xyzw ACC, vf05, vf09y */
    FMAC(OP_MADD, 15, ACC, 6, 9, 2); NEXT(); /* 0678 maddaz.xyzw ACC, vf06, vf09z */
    FMAC(OP_MADD, 15, 25, 7, 0, 3); NEXT(); /* 0680 maddw.xyzw vf25, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); SQI(15, 30, 3); NEXT(); /* 0688 mulax.xyzw ACC, vf04, vf10x | sqi.xyzw vf30, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); NEXT(); /* 0690 madday.xyzw ACC, vf05, vf10y */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); NEXT(); /* 0698 maddaz.xyzw ACC, vf06, vf10z */
    FMAC(OP_MADD, 15, 26, 7, 0, 3); NEXT(); /* 06a0 maddw.xyzw vf26, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 11, 0); SQI(15, 31, 3); NEXT(); /* 06a8 mulax.xyzw ACC, vf04, vf11x | sqi.xyzw vf31, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 11, 1); NEXT(); /* 06b0 madday.xyzw ACC, vf05, vf11y */
    FMAC(OP_MADD, 15, ACC, 6, 11, 2); NEXT(); /* 06b8 [E] maddaz.xyzw ACC, vf06, vf11z */
    FMAC(OP_MADD, 15, 27, 7, 0, 3); NEXT(); /* 06c0 maddw.xyzw vf27, vf07, vf00w */
}

static void k_06c8(void) {
    FMAC(OP_MUL, 15, ACC, 4, 12, 0); SQI(15, 24, 3); NEXT(); /* 06c8 mulax.xyzw ACC, vf04, vf12x | sqi.xyzw vf24, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 12, 1); NEXT(); /* 06d0 madday.xyzw ACC, vf05, vf12y */
    FMAC(OP_MADD, 15, ACC, 6, 12, 2); NEXT(); /* 06d8 maddaz.xyzw ACC, vf06, vf12z */
    FMAC(OP_MADD, 15, 28, 7, 0, 3); NEXT(); /* 06e0 maddw.xyzw vf28, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 13, 0); SQI(15, 25, 3); NEXT(); /* 06e8 mulax.xyzw ACC, vf04, vf13x | sqi.xyzw vf25, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 13, 1); NEXT(); /* 06f0 madday.xyzw ACC, vf05, vf13y */
    FMAC(OP_MADD, 15, ACC, 6, 13, 2); NEXT(); /* 06f8 maddaz.xyzw ACC, vf06, vf13z */
    FMAC(OP_MADD, 15, 29, 7, 0, 3); NEXT(); /* 0700 maddw.xyzw vf29, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 14, 0); SQI(15, 26, 3); NEXT(); /* 0708 mulax.xyzw ACC, vf04, vf14x | sqi.xyzw vf26, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 14, 1); NEXT(); /* 0710 madday.xyzw ACC, vf05, vf14y */
    FMAC(OP_MADD, 15, ACC, 6, 14, 2); NEXT(); /* 0718 maddaz.xyzw ACC, vf06, vf14z */
    FMAC(OP_MADD, 15, 30, 7, 0, 3); NEXT(); /* 0720 maddw.xyzw vf30, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 15, 0); SQI(15, 27, 3); NEXT(); /* 0728 mulax.xyzw ACC, vf04, vf15x | sqi.xyzw vf27, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 15, 1); NEXT(); /* 0730 madday.xyzw ACC, vf05, vf15y */
    FMAC(OP_MADD, 15, ACC, 6, 15, 2); NEXT(); /* 0738 [E] maddaz.xyzw ACC, vf06, vf15z */
    FMAC(OP_MADD, 15, 31, 7, 0, 3); NEXT(); /* 0740 maddw.xyzw vf31, vf07, vf00w */
}

static void k_0748(void) {
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); SQI(3, 16, 3); NEXT(); /* 0748 mulax.xyzw ACC, vf04, vf10x | sqi.xy vf16, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); SQI(3, 17, 3); NEXT(); /* 0750 madday.xyzw ACC, vf05, vf10y | sqi.xy vf17, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); SQI(3, 18, 3); NEXT(); /* 0758 maddaz.xyzw ACC, vf06, vf10z | sqi.xy vf18, (vi03++) */
    FMAC(OP_MADD, 15, 14, 7, 10, 3); SQI(3, 19, 3); NEXT(); /* 0760 maddw.xyzw vf14, vf07, vf10w | sqi.xy vf19, (vi03++) */
    FMAC(OP_MUL, 3, ACC, 8, 11, 0); IADDI(1, 0, -1); NEXT(); /* 0768 mulax.xy ACC, vf08, vf11x | iaddi vi01, vi00, -0x1 */
    FMAC(OP_MADD, 3, 15, 9, 11, 1); IADDI(6, 0, 1008); NEXT(); /* 0770 maddy.xy vf15, vf09, vf11y | iaddiu vi06, vi00, 0x3f0 */
    FMAC(OP_MSUB, 3, 16, 9, 11, 1); IADDI(7, 0, 15); NEXT(); /* 0778 msuby.xy vf16, vf09, vf11y | iaddiu vi07, vi00, 0xf */
    MR32(4, 3, 3); NEXT(); /* 0780 mr32.z vf03, vf03 */
    FMAC(OP_MAX, 4, 3, 14, 3, BC_VEC); MOVE(12, 24, 14); NEXT(); /* 0788 max.z vf03, vf14, vf03 | move.zw vf24, vf14 */
    MOVE(12, 25, 14); NEXT(); /* 0790 move.zw vf25, vf14 */
    MOVE(12, 26, 14); NEXT(); /* 0798 move.zw vf26, vf14 */
    MOVE(12, 27, 14); NEXT(); /* 07a0 move.zw vf27, vf14 */
    DIV(1, 3, 3, 2); NEXT(); /* 07a8 div Q, vf01w, vf03z */
    FMAC(OP_SUB, 3, 24, 14, 15, BC_VEC); ISUB(4, 1, 8); NEXT(); /* 07b0 sub.xy vf24, vf14, vf15 | isub vi04, vi01, vi08 */
    FMAC(OP_SUB, 3, 25, 14, 16, BC_VEC); IAND(4, 4, 6); NEXT(); /* 07b8 sub.xy vf25, vf14, vf16 | iand vi04, vi04, vi06 */
    FMAC(OP_ADD, 3, 26, 14, 15, BC_VEC); IAND(5, 8, 7); NEXT(); /* 07c0 add.xy vf26, vf14, vf15 | iand vi05, vi08, vi07 */
    FMAC(OP_ADD, 3, 27, 14, 16, BC_VEC); IOR(8, 4, 5); NEXT(); /* 07c8 add.xy vf27, vf14, vf16 | ior vi08, vi04, vi05 */
    FMAC(OP_MUL, 7, 16, 24, 1, BC_VEC); MFIR(8, 28, 8); NEXT(); /* 07d0 mul.xyz vf16, vf24, vf01 | mfir.w vf28, vi08 */
    FMAC(OP_MUL, 7, 17, 25, 1, BC_VEC); ISUB(4, 1, 9); NEXT(); /* 07d8 mul.xyz vf17, vf25, vf01 | isub vi04, vi01, vi09 */
    FMAC(OP_MUL, 7, 18, 26, 1, BC_VEC); IAND(4, 4, 6); NEXT(); /* 07e0 mul.xyz vf18, vf26, vf01 | iand vi04, vi04, vi06 */
    FMAC(OP_MUL, 7, 19, 27, 1, BC_VEC); IAND(5, 9, 7); NEXT(); /* 07e8 mul.xyz vf19, vf27, vf01 | iand vi05, vi09, vi07 */
    FMAC(OP_MUL, 3, 16, 16, 0, BC_Q); IOR(9, 4, 5); NEXT(); /* 07f0 mulq.xy vf16, vf16, Q | ior vi09, vi04, vi05 */
    FMAC(OP_MUL, 3, 17, 17, 0, BC_Q); MFIR(8, 29, 9); NEXT(); /* 07f8 mulq.xy vf17, vf17, Q | mfir.w vf29, vi09 */
    FMAC(OP_MUL, 3, 18, 18, 0, BC_Q); ISUB(4, 1, 10); NEXT(); /* 0800 mulq.xy vf18, vf18, Q | isub vi04, vi01, vi10 */
    FMAC(OP_MUL, 3, 19, 19, 0, BC_Q); IAND(4, 4, 6); NEXT(); /* 0808 mulq.xy vf19, vf19, Q | iand vi04, vi04, vi06 */
    FMAC(OP_ADD, 7, 16, 16, 2, BC_VEC); IAND(5, 10, 7); NEXT(); /* 0810 add.xyz vf16, vf16, vf02 | iand vi05, vi10, vi07 */
    FMAC(OP_ADD, 7, 17, 17, 2, BC_VEC); IOR(10, 4, 5); NEXT(); /* 0818 add.xyz vf17, vf17, vf02 | ior vi10, vi04, vi05 */
    FMAC(OP_ADD, 7, 18, 18, 2, BC_VEC); MFIR(8, 30, 10); NEXT(); /* 0820 add.xyz vf18, vf18, vf02 | mfir.w vf30, vi10 */
    FMAC(OP_ADD, 7, 19, 19, 2, BC_VEC); ISUB(4, 1, 11); NEXT(); /* 0828 add.xyz vf19, vf19, vf02 | isub vi04, vi01, vi11 */
    CLIP(16, 0); IAND(4, 4, 6); NEXT(); /* 0830 clipw.xyz vf16, vf00w | iand vi04, vi04, vi06 */
    CLIP(16, 2); IAND(5, 11, 7); NEXT(); /* 0838 clipw.xyz vf16, vf02w | iand vi05, vi11, vi07 */
    CLIP(17, 0); IOR(11, 4, 5); NEXT(); /* 0840 clipw.xyz vf17, vf00w | ior vi11, vi04, vi05 */
    CLIP(17, 2); MFIR(8, 31, 11); NEXT(); /* 0848 clipw.xyz vf17, vf02w | mfir.w vf31, vi11 */
    CLIP(18, 0); NEXT(); /* 0850 clipw.xyz vf18, vf00w */
    CLIP(18, 2); FCGET(8); NEXT(); /* 0858 clipw.xyz vf18, vf02w | fcget vi08 */
    CLIP(19, 0); NEXT(); /* 0860 clipw.xyz vf19, vf00w */
    CLIP(19, 2); FCGET(9); NEXT(); /* 0868 clipw.xyz vf19, vf02w | fcget vi09 */
    NEXT(); /* 0870 nop */
    FCGET(10); NEXT(); /* 0878 fcget vi10 */
    NEXT(); /* 0880 [E] nop */
    FCGET(11); NEXT(); /* 0888 fcget vi11 */
}

static void k_0890(void) {
    FMAC(OP_MUL, 15, ACC, 4, 12, 0); SQI(3, 16, 3); NEXT(); /* 0890 mulax.xyzw ACC, vf04, vf12x | sqi.xy vf16, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 5, 12, 1); SQI(3, 17, 3); NEXT(); /* 0898 madday.xyzw ACC, vf05, vf12y | sqi.xy vf17, (vi03++) */
    FMAC(OP_MADD, 15, ACC, 6, 12, 2); SQI(3, 18, 3); NEXT(); /* 08a0 maddaz.xyzw ACC, vf06, vf12z | sqi.xy vf18, (vi03++) */
    FMAC(OP_MADD, 15, 14, 7, 12, 3); SQI(3, 19, 3); NEXT(); /* 08a8 maddw.xyzw vf14, vf07, vf12w | sqi.xy vf19, (vi03++) */
    FMAC(OP_MUL, 3, ACC, 8, 13, 0); IADDI(1, 0, -1); NEXT(); /* 08b0 mulax.xy ACC, vf08, vf13x | iaddi vi01, vi00, -0x1 */
    FMAC(OP_MADD, 3, 15, 9, 13, 1); IADDI(6, 0, 1008); NEXT(); /* 08b8 maddy.xy vf15, vf09, vf13y | iaddiu vi06, vi00, 0x3f0 */
    FMAC(OP_MSUB, 3, 16, 9, 13, 1); IADDI(7, 0, 15); NEXT(); /* 08c0 msuby.xy vf16, vf09, vf13y | iaddiu vi07, vi00, 0xf */
    MR32(4, 3, 3); NEXT(); /* 08c8 mr32.z vf03, vf03 */
    FMAC(OP_MAX, 4, 3, 14, 3, BC_VEC); MOVE(12, 28, 14); NEXT(); /* 08d0 max.z vf03, vf14, vf03 | move.zw vf28, vf14 */
    MOVE(12, 29, 14); NEXT(); /* 08d8 move.zw vf29, vf14 */
    MOVE(12, 30, 14); NEXT(); /* 08e0 move.zw vf30, vf14 */
    MOVE(12, 31, 14); NEXT(); /* 08e8 move.zw vf31, vf14 */
    DIV(1, 3, 3, 2); NEXT(); /* 08f0 div Q, vf01w, vf03z */
    FMAC(OP_SUB, 3, 28, 14, 15, BC_VEC); ISUB(4, 1, 8); NEXT(); /* 08f8 sub.xy vf28, vf14, vf15 | isub vi04, vi01, vi08 */
    FMAC(OP_SUB, 3, 29, 14, 16, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0900 sub.xy vf29, vf14, vf16 | iand vi04, vi04, vi06 */
    FMAC(OP_ADD, 3, 30, 14, 15, BC_VEC); IAND(5, 8, 7); NEXT(); /* 0908 add.xy vf30, vf14, vf15 | iand vi05, vi08, vi07 */
    FMAC(OP_ADD, 3, 31, 14, 16, BC_VEC); IOR(8, 4, 5); NEXT(); /* 0910 add.xy vf31, vf14, vf16 | ior vi08, vi04, vi05 */
    FMAC(OP_MUL, 7, 16, 28, 1, BC_VEC); MFIR(8, 24, 8); NEXT(); /* 0918 mul.xyz vf16, vf28, vf01 | mfir.w vf24, vi08 */
    FMAC(OP_MUL, 7, 17, 29, 1, BC_VEC); ISUB(4, 1, 9); NEXT(); /* 0920 mul.xyz vf17, vf29, vf01 | isub vi04, vi01, vi09 */
    FMAC(OP_MUL, 7, 18, 30, 1, BC_VEC); IAND(4, 4, 6); NEXT(); /* 0928 mul.xyz vf18, vf30, vf01 | iand vi04, vi04, vi06 */
    FMAC(OP_MUL, 7, 19, 31, 1, BC_VEC); IAND(5, 9, 7); NEXT(); /* 0930 mul.xyz vf19, vf31, vf01 | iand vi05, vi09, vi07 */
    FMAC(OP_MUL, 3, 16, 16, 0, BC_Q); IOR(9, 4, 5); NEXT(); /* 0938 mulq.xy vf16, vf16, Q | ior vi09, vi04, vi05 */
    FMAC(OP_MUL, 3, 17, 17, 0, BC_Q); MFIR(8, 25, 9); NEXT(); /* 0940 mulq.xy vf17, vf17, Q | mfir.w vf25, vi09 */
    FMAC(OP_MUL, 3, 18, 18, 0, BC_Q); ISUB(4, 1, 10); NEXT(); /* 0948 mulq.xy vf18, vf18, Q | isub vi04, vi01, vi10 */
    FMAC(OP_MUL, 3, 19, 19, 0, BC_Q); IAND(4, 4, 6); NEXT(); /* 0950 mulq.xy vf19, vf19, Q | iand vi04, vi04, vi06 */
    FMAC(OP_ADD, 7, 16, 16, 2, BC_VEC); IAND(5, 10, 7); NEXT(); /* 0958 add.xyz vf16, vf16, vf02 | iand vi05, vi10, vi07 */
    FMAC(OP_ADD, 7, 17, 17, 2, BC_VEC); IOR(10, 4, 5); NEXT(); /* 0960 add.xyz vf17, vf17, vf02 | ior vi10, vi04, vi05 */
    FMAC(OP_ADD, 7, 18, 18, 2, BC_VEC); MFIR(8, 26, 10); NEXT(); /* 0968 add.xyz vf18, vf18, vf02 | mfir.w vf26, vi10 */
    FMAC(OP_ADD, 7, 19, 19, 2, BC_VEC); ISUB(4, 1, 11); NEXT(); /* 0970 add.xyz vf19, vf19, vf02 | isub vi04, vi01, vi11 */
    CLIP(16, 0); IAND(4, 4, 6); NEXT(); /* 0978 clipw.xyz vf16, vf00w | iand vi04, vi04, vi06 */
    CLIP(16, 2); IAND(5, 11, 7); NEXT(); /* 0980 clipw.xyz vf16, vf02w | iand vi05, vi11, vi07 */
    CLIP(17, 0); IOR(11, 4, 5); NEXT(); /* 0988 clipw.xyz vf17, vf00w | ior vi11, vi04, vi05 */
    CLIP(17, 2); MFIR(8, 27, 11); NEXT(); /* 0990 clipw.xyz vf17, vf02w | mfir.w vf27, vi11 */
    CLIP(18, 0); MR32(4, 3, 3); NEXT(); /* 0998 clipw.xyz vf18, vf00w | mr32.z vf03, vf03 */
    CLIP(18, 2); FCGET(8); NEXT(); /* 09a0 clipw.xyz vf18, vf02w | fcget vi08 */
    CLIP(19, 0); NEXT(); /* 09a8 clipw.xyz vf19, vf00w */
    CLIP(19, 2); FCGET(9); NEXT(); /* 09b0 clipw.xyz vf19, vf02w | fcget vi09 */
    NEXT(); /* 09b8 nop */
    FCGET(10); NEXT(); /* 09c0 fcget vi10 */
    NEXT(); /* 09c8 [E] nop */
    FCGET(11); NEXT(); /* 09d0 fcget vi11 */
}

static void k_09d8(void) {
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); NEXT(); /* 09d8 mulax.xyzw ACC, vf04, vf10x */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); NEXT(); /* 09e0 madday.xyzw ACC, vf05, vf10y */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); NEXT(); /* 09e8 maddaz.xyzw ACC, vf06, vf10z */
    FMAC(OP_MADD, 15, 14, 7, 10, 3); NEXT(); /* 09f0 maddw.xyzw vf14, vf07, vf10w */
    FMAC(OP_MUL, 3, ACC, 8, 11, 0); NEXT(); /* 09f8 mulax.xy ACC, vf08, vf11x */
    FMAC(OP_MADD, 3, 15, 9, 11, 1); NEXT(); /* 0a00 maddy.xy vf15, vf09, vf11y */
    FMAC(OP_MSUB, 3, 16, 9, 11, 1); NEXT(); /* 0a08 msuby.xy vf16, vf09, vf11y */
    FMAC(OP_SUB, 3, 24, 14, 15, BC_VEC); MOVE(12, 25, 14); NEXT(); /* 0a10 sub.xy vf24, vf14, vf15 | move.zw vf25, vf14 */
    FMAC(OP_ADD, 3, 26, 14, 15, BC_VEC); MOVE(12, 27, 14); NEXT(); /* 0a18 add.xy vf26, vf14, vf15 | move.zw vf27, vf14 */
    FMAC(OP_SUB, 3, 25, 14, 16, BC_VEC); MOVE(12, 24, 14); NEXT(); /* 0a20 sub.xy vf25, vf14, vf16 | move.zw vf24, vf14 */
    FMAC(OP_ADD, 3, 27, 14, 16, BC_VEC); MOVE(12, 26, 14); NEXT(); /* 0a28 add.xy vf27, vf14, vf16 | move.zw vf26, vf14 */
    NEXT(); /* 0a30 [E] nop */
    NEXT(); /* 0a38 nop */
}

static void k_0a40(void) {
    FMAC(OP_MUL, 15, ACC, 4, 12, 0); NEXT(); /* 0a40 mulax.xyzw ACC, vf04, vf12x */
    FMAC(OP_MADD, 15, ACC, 5, 12, 1); NEXT(); /* 0a48 madday.xyzw ACC, vf05, vf12y */
    FMAC(OP_MADD, 15, ACC, 6, 12, 2); NEXT(); /* 0a50 maddaz.xyzw ACC, vf06, vf12z */
    FMAC(OP_MADD, 15, 14, 7, 12, 3); NEXT(); /* 0a58 maddw.xyzw vf14, vf07, vf12w */
    FMAC(OP_MUL, 3, ACC, 8, 13, 0); NEXT(); /* 0a60 mulax.xy ACC, vf08, vf13x */
    FMAC(OP_MADD, 3, 15, 9, 13, 1); NEXT(); /* 0a68 maddy.xy vf15, vf09, vf13y */
    FMAC(OP_MSUB, 3, 16, 9, 13, 1); NEXT(); /* 0a70 msuby.xy vf16, vf09, vf13y */
    FMAC(OP_SUB, 3, 28, 14, 15, BC_VEC); MOVE(12, 29, 14); NEXT(); /* 0a78 sub.xy vf28, vf14, vf15 | move.zw vf29, vf14 */
    FMAC(OP_ADD, 3, 30, 14, 15, BC_VEC); MOVE(12, 31, 14); NEXT(); /* 0a80 add.xy vf30, vf14, vf15 | move.zw vf31, vf14 */
    FMAC(OP_SUB, 3, 29, 14, 16, BC_VEC); MOVE(12, 28, 14); NEXT(); /* 0a88 sub.xy vf29, vf14, vf16 | move.zw vf28, vf14 */
    FMAC(OP_ADD, 3, 31, 14, 16, BC_VEC); MOVE(12, 30, 14); NEXT(); /* 0a90 add.xy vf31, vf14, vf16 | move.zw vf30, vf14 */
    NEXT(); /* 0a98 [E] nop */
    NEXT(); /* 0aa0 nop */
}

static void k_0aa8(void) {
    FMAC(OP_SUB, 7, 7, 2, 1, BC_VEC); IADDI(1, 0, 128); NEXT(); /* 0aa8 sub.xyz vf07, vf02, vf01 | iaddiu vi01, vi00, 0x80 */
    FMAC(OP_SUB, 7, 8, 3, 1, BC_VEC); NEXT(); /* 0ab0 sub.xyz vf08, vf03, vf01 */
    FMAC(OP_ADD, 7, 31, 0, 0, 3); MOVE(15, 10, 1); NEXT(); /* 0ab8 addw.xyz vf31, vf00, vf00w | move.xyzw vf10, vf01 */
    OPMULA(7, 8); NEXT(); /* 0ac0 opmula.xyz ACC, vf07, vf08 */
    OPMSUB(9, 8, 7); NEXT(); /* 0ac8 opmsub.xyz vf09, vf08, vf07 */
    FMAC(OP_MUL, 7, 9, 9, 10, BC_VEC); NEXT(); /* 0ad0 mul.xyz vf09, vf09, vf10 */
    FMAC(OP_ADD, 1, ACC, 9, 9, 1); NEXT(); /* 0ad8 adday.x ACC, vf09, vf09y */
    FMAC(OP_MADD, 1, ACC, 31, 9, 2); NEXT(); /* 0ae0 maddaz.x ACC, vf31, vf09z */
    NEXT(); /* 0ae8 nop */
    NEXT(); /* 0af0 nop */
    NEXT(); /* 0af8 [E] nop */
    FMAND(2, 1); NEXT(); /* 0b00 fmand vi02, vi01 */
}

static void k_0b08(void) {
    FMAC(OP_SUB, 7, 11, 5, 4, BC_VEC); IADDI(1, 0, 128); NEXT(); /* 0b08 sub.xyz vf11, vf05, vf04 | iaddiu vi01, vi00, 0x80 */
    FMAC(OP_SUB, 7, 12, 6, 4, BC_VEC); NEXT(); /* 0b10 sub.xyz vf12, vf06, vf04 */
    FMAC(OP_ADD, 7, 31, 0, 0, 3); MOVE(15, 14, 4); NEXT(); /* 0b18 addw.xyz vf31, vf00, vf00w | move.xyzw vf14, vf04 */
    OPMULA(11, 12); NEXT(); /* 0b20 opmula.xyz ACC, vf11, vf12 */
    OPMSUB(13, 12, 11); NEXT(); /* 0b28 opmsub.xyz vf13, vf12, vf11 */
    FMAC(OP_MUL, 7, 13, 13, 14, BC_VEC); NEXT(); /* 0b30 mul.xyz vf13, vf13, vf14 */
    FMAC(OP_ADD, 1, ACC, 13, 13, 1); NEXT(); /* 0b38 adday.x ACC, vf13, vf13y */
    FMAC(OP_MADD, 1, ACC, 31, 13, 2); NEXT(); /* 0b40 maddaz.x ACC, vf31, vf13z */
    NEXT(); /* 0b48 nop */
    NEXT(); /* 0b50 nop */
    NEXT(); /* 0b58 [E] nop */
    FMAND(3, 1); NEXT(); /* 0b60 fmand vi03, vi01 */
}

static void k_0b68(void) {
    FMAC(OP_SUB, 7, 13, 2, 1, BC_VEC); IADDI(13, 0, 136); NEXT(); /* 0b68 sub.xyz vf13, vf02, vf01 | iaddiu vi13, vi00, 0x88 */
    FMAC(OP_SUB, 7, 14, 3, 1, BC_VEC); IADDI(14, 0, 127); NEXT(); /* 0b70 sub.xyz vf14, vf03, vf01 | iaddiu vi14, vi00, 0x7f */
    FMAC(OP_SUB, 7, 15, 5, 4, BC_VEC); MTIR(1, 27, 0); NEXT(); /* 0b78 sub.xyz vf15, vf05, vf04 | mtir vi01, vf27x */
    FMAC(OP_SUB, 7, 16, 6, 4, BC_VEC); MTIR(2, 27, 1); NEXT(); /* 0b80 sub.xyz vf16, vf06, vf04 | mtir vi02, vf27y */
    FMAC(OP_ADD, 7, 31, 0, 0, 3); MTIR(3, 27, 2); NEXT(); /* 0b88 addw.xyz vf31, vf00, vf00w | mtir vi03, vf27z */
    OPMULA(13, 14); MTIR(4, 28, 0); NEXT(); /* 0b90 opmula.xyz ACC, vf13, vf14 | mtir vi04, vf28x */
    OPMSUB(21, 14, 13); MTIR(5, 28, 1); NEXT(); /* 0b98 opmsub.xyz vf21, vf14, vf13 | mtir vi05, vf28y */
    OPMULA(15, 16); MTIR(6, 28, 2); NEXT(); /* 0ba0 opmula.xyz ACC, vf15, vf16 | mtir vi06, vf28z */
    OPMSUB(22, 16, 15); IAND(1, 1, 14); NEXT(); /* 0ba8 opmsub.xyz vf22, vf16, vf15 | iand vi01, vi01, vi14 */
    IAND(2, 2, 14); NEXT(); /* 0bb0 iand vi02, vi02, vi14 */
    FMAC(OP_MUL, 7, 21, 21, 1, BC_VEC); IAND(3, 3, 14); NEXT(); /* 0bb8 mul.xyz vf21, vf21, vf01 | iand vi03, vi03, vi14 */
    IAND(4, 4, 14); NEXT(); /* 0bc0 iand vi04, vi04, vi14 */
    FMAC(OP_MUL, 7, 22, 22, 4, BC_VEC); IAND(5, 5, 14); NEXT(); /* 0bc8 mul.xyz vf22, vf22, vf04 | iand vi05, vi05, vi14 */
    IAND(6, 6, 14); NEXT(); /* 0bd0 iand vi06, vi06, vi14 */
    FMAC(OP_ADD, 1, ACC, 21, 21, 1); LQ(15, 7, 4, 1); NEXT(); /* 0bd8 adday.x ACC, vf21, vf21y | lq.xyzw vf07, 0x4(vi01) */
    FMAC(OP_MADD, 1, ACC, 31, 21, 2); LQ(15, 8, 4, 2); NEXT(); /* 0be0 maddaz.x ACC, vf31, vf21z | lq.xyzw vf08, 0x4(vi02) */
    FMAC(OP_ADD, 1, ACC, 22, 22, 1); LQ(15, 9, 4, 3); NEXT(); /* 0be8 adday.x ACC, vf22, vf22y | lq.xyzw vf09, 0x4(vi03) */
    FMAC(OP_MADD, 1, ACC, 31, 22, 2); LQ(15, 10, 4, 4); NEXT(); /* 0bf0 maddaz.x ACC, vf31, vf22z | lq.xyzw vf10, 0x4(vi04) */
    LQ(15, 11, 4, 5); NEXT(); /* 0bf8 lq.xyzw vf11, 0x4(vi05) */
    FMAND(2, 13); NEXT(); /* 0c00 fmand vi02, vi13 */
    LQ(15, 12, 4, 6); NEXT(); /* 0c08 [E] lq.xyzw vf12, 0x4(vi06) */
    FMAND(3, 13); NEXT(); /* 0c10 fmand vi03, vi13 */
}

static void k_0c18(void) {
    FMAC(OP_SUB, 7, 13, 8, 7, BC_VEC); IADDI(13, 0, 136); NEXT(); /* 0c18 sub.xyz vf13, vf08, vf07 | iaddiu vi13, vi00, 0x88 */
    FMAC(OP_SUB, 7, 14, 9, 7, BC_VEC); IADDI(14, 0, 127); NEXT(); /* 0c20 sub.xyz vf14, vf09, vf07 | iaddiu vi14, vi00, 0x7f */
    FMAC(OP_SUB, 7, 15, 11, 10, BC_VEC); MTIR(7, 25, 0); NEXT(); /* 0c28 sub.xyz vf15, vf11, vf10 | mtir vi07, vf25x */
    FMAC(OP_SUB, 7, 16, 12, 10, BC_VEC); MTIR(8, 25, 1); NEXT(); /* 0c30 sub.xyz vf16, vf12, vf10 | mtir vi08, vf25y */
    FMAC(OP_ADD, 7, 31, 0, 0, 3); MTIR(9, 25, 2); NEXT(); /* 0c38 addw.xyz vf31, vf00, vf00w | mtir vi09, vf25z */
    OPMULA(13, 14); MTIR(10, 26, 0); NEXT(); /* 0c40 opmula.xyz ACC, vf13, vf14 | mtir vi10, vf26x */
    OPMSUB(21, 14, 13); MTIR(11, 26, 1); NEXT(); /* 0c48 opmsub.xyz vf21, vf14, vf13 | mtir vi11, vf26y */
    OPMULA(15, 16); MTIR(12, 26, 2); NEXT(); /* 0c50 opmula.xyz ACC, vf15, vf16 | mtir vi12, vf26z */
    OPMSUB(22, 16, 15); IAND(7, 7, 14); NEXT(); /* 0c58 opmsub.xyz vf22, vf16, vf15 | iand vi07, vi07, vi14 */
    IAND(8, 8, 14); NEXT(); /* 0c60 iand vi08, vi08, vi14 */
    FMAC(OP_MUL, 7, 21, 21, 7, BC_VEC); IAND(9, 9, 14); NEXT(); /* 0c68 mul.xyz vf21, vf21, vf07 | iand vi09, vi09, vi14 */
    IAND(10, 10, 14); NEXT(); /* 0c70 iand vi10, vi10, vi14 */
    FMAC(OP_MUL, 7, 22, 22, 10, BC_VEC); IAND(11, 11, 14); NEXT(); /* 0c78 mul.xyz vf22, vf22, vf10 | iand vi11, vi11, vi14 */
    IAND(12, 12, 14); NEXT(); /* 0c80 iand vi12, vi12, vi14 */
    FMAC(OP_ADD, 1, ACC, 21, 21, 1); LQ(15, 1, 4, 7); NEXT(); /* 0c88 adday.x ACC, vf21, vf21y | lq.xyzw vf01, 0x4(vi07) */
    FMAC(OP_MADD, 1, ACC, 31, 21, 2); LQ(15, 2, 4, 8); NEXT(); /* 0c90 maddaz.x ACC, vf31, vf21z | lq.xyzw vf02, 0x4(vi08) */
    FMAC(OP_ADD, 1, ACC, 22, 22, 1); LQ(15, 3, 4, 9); NEXT(); /* 0c98 adday.x ACC, vf22, vf22y | lq.xyzw vf03, 0x4(vi09) */
    FMAC(OP_MADD, 1, ACC, 31, 22, 2); LQ(15, 4, 4, 10); NEXT(); /* 0ca0 maddaz.x ACC, vf31, vf22z | lq.xyzw vf04, 0x4(vi10) */
    LQ(15, 5, 4, 11); NEXT(); /* 0ca8 lq.xyzw vf05, 0x4(vi11) */
    FMAND(4, 13); NEXT(); /* 0cb0 fmand vi04, vi13 */
    LQ(15, 6, 4, 12); NEXT(); /* 0cb8 [E] lq.xyzw vf06, 0x4(vi12) */
    FMAND(5, 13); NEXT(); /* 0cc0 fmand vi05, vi13 */
}

static void k_0cc8(void) {
    FMAC(OP_MUL, 15, ACC, 4, 8, 0); NEXT(); /* 0cc8 mulax.xyzw ACC, vf04, vf08x */
    FMAC(OP_MADD, 15, ACC, 5, 8, 1); NEXT(); /* 0cd0 madday.xyzw ACC, vf05, vf08y */
    FMAC(OP_MADD, 15, ACC, 6, 8, 2); NEXT(); /* 0cd8 maddaz.xyzw ACC, vf06, vf08z */
    FMAC(OP_MADD, 15, 24, 7, 0, 3); NEXT(); /* 0ce0 maddw.xyzw vf24, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 9, 0); NEXT(); /* 0ce8 mulax.xyzw ACC, vf04, vf09x */
    FMAC(OP_MADD, 15, ACC, 5, 9, 1); NEXT(); /* 0cf0 madday.xyzw ACC, vf05, vf09y */
    FMAC(OP_MADD, 15, ACC, 6, 9, 2); NEXT(); /* 0cf8 maddaz.xyzw ACC, vf06, vf09z */
    FMAC(OP_MADD, 15, 25, 7, 0, 3); SQI(15, 24, 3); NEXT(); /* 0d00 maddw.xyzw vf25, vf07, vf00w | sqi.xyzw vf24, (vi03++) */
    FMAC(OP_MUL, 15, ACC, 4, 10, 0); NEXT(); /* 0d08 mulax.xyzw ACC, vf04, vf10x */
    FMAC(OP_MADD, 15, ACC, 5, 10, 1); NEXT(); /* 0d10 madday.xyzw ACC, vf05, vf10y */
    FMAC(OP_MADD, 15, ACC, 6, 10, 2); NEXT(); /* 0d18 maddaz.xyzw ACC, vf06, vf10z */
    FMAC(OP_MADD, 15, 26, 7, 0, 3); SQI(15, 25, 3); NEXT(); /* 0d20 maddw.xyzw vf26, vf07, vf00w | sqi.xyzw vf25, (vi03++) */
    FMAC(OP_MUL, 15, ACC, 4, 11, 0); NEXT(); /* 0d28 mulax.xyzw ACC, vf04, vf11x */
    FMAC(OP_MADD, 15, ACC, 5, 11, 1); NEXT(); /* 0d30 madday.xyzw ACC, vf05, vf11y */
    FMAC(OP_MADD, 15, ACC, 6, 11, 2); NEXT(); /* 0d38 maddaz.xyzw ACC, vf06, vf11z */
    FMAC(OP_MADD, 15, 27, 7, 0, 3); SQI(15, 26, 3); NEXT(); /* 0d40 maddw.xyzw vf27, vf07, vf00w | sqi.xyzw vf26, (vi03++) */
    FMAC(OP_ADD, 4, 24, 0, 25, 0); NEXT(); /* 0d48 addx.z vf24, vf00, vf25x */
    FMAC(OP_MUL, 8, 24, 0, 25, 1); NEXT(); /* 0d50 muly.w vf24, vf00, vf25y */
    FMAC(OP_ADD, 4, 26, 0, 27, 0); NEXT(); /* 0d58 [E] addx.z vf26, vf00, vf27x */
    FMAC(OP_MUL, 8, 26, 0, 27, 1); SQI(15, 27, 3); NEXT(); /* 0d60 muly.w vf26, vf00, vf27y | sqi.xyzw vf27, (vi03++) */
}

static void k_0d68(void) {
    FMAC(OP_MUL, 15, ACC, 4, 12, 0); NEXT(); /* 0d68 mulax.xyzw ACC, vf04, vf12x */
    FMAC(OP_MADD, 15, ACC, 5, 12, 1); NEXT(); /* 0d70 madday.xyzw ACC, vf05, vf12y */
    FMAC(OP_MADD, 15, ACC, 6, 12, 2); NEXT(); /* 0d78 maddaz.xyzw ACC, vf06, vf12z */
    FMAC(OP_MADD, 15, 28, 7, 0, 3); NEXT(); /* 0d80 maddw.xyzw vf28, vf07, vf00w */
    FMAC(OP_MUL, 15, ACC, 4, 13, 0); NEXT(); /* 0d88 mulax.xyzw ACC, vf04, vf13x */
    FMAC(OP_MADD, 15, ACC, 5, 13, 1); NEXT(); /* 0d90 madday.xyzw ACC, vf05, vf13y */
    FMAC(OP_MADD, 15, ACC, 6, 13, 2); NEXT(); /* 0d98 maddaz.xyzw ACC, vf06, vf13z */
    FMAC(OP_MADD, 15, 29, 7, 0, 3); SQI(15, 28, 3); NEXT(); /* 0da0 maddw.xyzw vf29, vf07, vf00w | sqi.xyzw vf28, (vi03++) */
    FMAC(OP_MUL, 15, ACC, 4, 14, 0); NEXT(); /* 0da8 mulax.xyzw ACC, vf04, vf14x */
    FMAC(OP_MADD, 15, ACC, 5, 14, 1); NEXT(); /* 0db0 madday.xyzw ACC, vf05, vf14y */
    FMAC(OP_MADD, 15, ACC, 6, 14, 2); NEXT(); /* 0db8 maddaz.xyzw ACC, vf06, vf14z */
    FMAC(OP_MADD, 15, 30, 7, 0, 3); SQI(15, 29, 3); NEXT(); /* 0dc0 maddw.xyzw vf30, vf07, vf00w | sqi.xyzw vf29, (vi03++) */
    FMAC(OP_MUL, 15, ACC, 4, 15, 0); NEXT(); /* 0dc8 mulax.xyzw ACC, vf04, vf15x */
    FMAC(OP_MADD, 15, ACC, 5, 15, 1); NEXT(); /* 0dd0 madday.xyzw ACC, vf05, vf15y */
    FMAC(OP_MADD, 15, ACC, 6, 15, 2); NEXT(); /* 0dd8 maddaz.xyzw ACC, vf06, vf15z */
    FMAC(OP_MADD, 15, 31, 7, 0, 3); SQI(15, 30, 3); NEXT(); /* 0de0 maddw.xyzw vf31, vf07, vf00w | sqi.xyzw vf30, (vi03++) */
    FMAC(OP_ADD, 4, 28, 0, 29, 0); NEXT(); /* 0de8 addx.z vf28, vf00, vf29x */
    FMAC(OP_MUL, 8, 28, 0, 29, 1); NEXT(); /* 0df0 muly.w vf28, vf00, vf29y */
    FMAC(OP_ADD, 4, 30, 0, 31, 0); NEXT(); /* 0df8 [E] addx.z vf30, vf00, vf31x */
    FMAC(OP_MUL, 8, 30, 0, 31, 1); SQI(15, 31, 3); NEXT(); /* 0e00 muly.w vf30, vf00, vf31y | sqi.xyzw vf31, (vi03++) */
}

static void k_0e08(void) {
    FMAC(OP_SUB, 7, 10, 5, 4, BC_VEC); MTIR(2, 1, 0); NEXT(); /* 0e08 sub.xyz vf10, vf05, vf04 | mtir vi02, vf01x */
    FMAC(OP_SUB, 7, 11, 6, 4, BC_VEC); MTIR(3, 1, 1); NEXT(); /* 0e10 sub.xyz vf11, vf06, vf04 | mtir vi03, vf01y */
    CLIP(4, 3); MTIR(4, 1, 2); NEXT(); /* 0e18 clipw.xyz vf04, vf03w | mtir vi04, vf01z */
    CLIP(5, 3); IADDI(10, 0, 63); NEXT(); /* 0e20 clipw.xyz vf05, vf03w | iaddiu vi10, vi00, 0x3f */
    CLIP(6, 3); LQ(15, 7, 0, 2); NEXT(); /* 0e28 clipw.xyz vf06, vf03w | lq.xyzw vf07, 0x0(vi02) */
    OPMULA(11, 10); LQ(15, 8, 0, 3); NEXT(); /* 0e30 opmula.xyz ACC, vf11, vf10 | lq.xyzw vf08, 0x0(vi03) */
    OPMSUB(12, 10, 11); FCGET(5); NEXT(); /* 0e38 opmsub.xyz vf12, vf10, vf11 | fcget vi05 */
    FCGET(6); NEXT(); /* 0e40 fcget vi06 */
    FCGET(7); NEXT(); /* 0e48 fcget vi07 */
    IAND(5, 5, 6); NEXT(); /* 0e50 iand vi05, vi05, vi06 */
    IAND(5, 5, 7); NEXT(); /* 0e58 iand vi05, vi05, vi07 */
    IAND(5, 5, 10); NEXT(); /* 0e60 iand vi05, vi05, vi10 */
    FMAND(11, 14); NEXT(); /* 0e68 fmand vi11, vi14 */
    IOR(11, 5, 11); NEXT(); /* 0e70 [E] ior vi11, vi05, vi11 */
    LQ(15, 9, 0, 4); NEXT(); /* 0e78 lq.xyzw vf09, 0x0(vi04) */
}

static void k_0e80(void) {
    FMAC(OP_SUB, 7, 10, 8, 7, BC_VEC); MTIR(2, 2, 0); NEXT(); /* 0e80 sub.xyz vf10, vf08, vf07 | mtir vi02, vf02x */
    FMAC(OP_SUB, 7, 11, 9, 7, BC_VEC); MTIR(3, 2, 1); NEXT(); /* 0e88 sub.xyz vf11, vf09, vf07 | mtir vi03, vf02y */
    CLIP(7, 3); MTIR(4, 2, 2); NEXT(); /* 0e90 clipw.xyz vf07, vf03w | mtir vi04, vf02z */
    CLIP(8, 3); IADDI(10, 0, 63); NEXT(); /* 0e98 clipw.xyz vf08, vf03w | iaddiu vi10, vi00, 0x3f */
    CLIP(9, 3); LQ(15, 4, 0, 2); NEXT(); /* 0ea0 clipw.xyz vf09, vf03w | lq.xyzw vf04, 0x0(vi02) */
    OPMULA(11, 10); LQ(15, 5, 0, 3); NEXT(); /* 0ea8 opmula.xyz ACC, vf11, vf10 | lq.xyzw vf05, 0x0(vi03) */
    OPMSUB(12, 10, 11); FCGET(7); NEXT(); /* 0eb0 opmsub.xyz vf12, vf10, vf11 | fcget vi07 */
    FCGET(8); NEXT(); /* 0eb8 fcget vi08 */
    FCGET(9); NEXT(); /* 0ec0 fcget vi09 */
    IAND(7, 7, 8); NEXT(); /* 0ec8 iand vi07, vi07, vi08 */
    IAND(7, 7, 9); NEXT(); /* 0ed0 iand vi07, vi07, vi09 */
    IAND(7, 7, 10); NEXT(); /* 0ed8 iand vi07, vi07, vi10 */
    FMAND(12, 14); NEXT(); /* 0ee0 fmand vi12, vi14 */
    IOR(12, 7, 12); NEXT(); /* 0ee8 [E] ior vi12, vi07, vi12 */
    LQ(15, 6, 0, 4); NEXT(); /* 0ef0 lq.xyzw vf06, 0x0(vi04) */
}

/* ---------------------------------------------------------------------- */

void grx_vu0_callms(unsigned int addr) {
    static unsigned int warned;
    void (*k)(void) = NULL;

    switch (addr) {
    case 0x0000: k = k_0000; break;
    case 0x0080: k = k_0080; break;
    case 0x0100: k = k_0100; break;
    case 0x0140: k = k_0140; break;
    case 0x01C0: k = k_01c0; break;
    case 0x0240: k = k_0240; break;
    case 0x02A0: k = k_02a0; break;
    case 0x0340: k = k_0340; break;
    case 0x03C8: k = k_03c8; break;
    case 0x0508: k = k_0508; break;
    case 0x0648: k = k_0648; break;
    case 0x06C8: k = k_06c8; break;
    case 0x0748: k = k_0748; break;
    case 0x0890: k = k_0890; break;
    case 0x09D8: k = k_09d8; break;
    case 0x0A40: k = k_0a40; break;
    case 0x0AA8: k = k_0aa8; break;
    case 0x0B08: k = k_0b08; break;
    case 0x0B68: k = k_0b68; break;
    case 0x0C18: k = k_0c18; break;
    case 0x0CC8: k = k_0cc8; break;
    case 0x0D68: k = k_0d68; break;
    case 0x0E08: k = k_0e08; break;
    case 0x0E80: k = k_0e80; break;
    default:
        break;
    }
    if (k == NULL) {
        if (warned++ == 0) {
            grx_log("vu0: vcallms 0x%X: no kernel there; further ones not reported", addr);
        }
        return;
    }
    begin();
    k();
    finish();
}

void grx_vu0_callmsr(void) {
    grx_vu0_callms((grx_vi[VI_CMSAR0] & 0xFFFF) * 8);
}
