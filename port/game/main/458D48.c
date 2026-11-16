/* Port-only C for the mesh pipeline's vertex transform stages, which the
   decompilation still has in assembly (asm/main/main/058D48.s):
   GphVertXformClip (0x00458D48), GphVertXform (0x00458E68),
   GphVertXformClipBillboard (0x00458F50) and GphVertXformBillboard
   (0x00459058). */
/*
 * Register contract (the drivers GphMeshDrawClip/Vu0/NoClip, port/game/main/
 * 4578B4.c, 457E7C.c, 458124.c): $a0 = the mesh, $a2 = where the vertex
 * packet goes (also in scratchpad 0x700000F4), $t9 = 0x70000000. Outputs:
 * the packet, its end (the transformed vertices' end) in 0x700000F8, and
 * VU0 data memory. No caller reads the registers the asm leaves, so only
 * memory is written here.
 *
 * The packet: a DMA CNT tag (qwc = n + 1, only its first word written),
 * at +0x10 a quadword of VIF codes (NOP, NOP, NOP, UNPACK V4-32 of n
 * quadwords to VU address 0; the packet builders put an MSCAL in its
 * first word), then from +0x20 the vertices, rounded up to a multiple of
 * 4 (the extra ones are made from whatever follows the input).
 *
 * The VU0 microprograms (vuMicroCode0, port/docs/vu.md, the listing from
 * port/tools/vu_disasm.py --game) are software-pipelined across their
 * A/B halves: each vcallms transforms 4 vertices while finishing the
 * projection and clip codes of the ones before, and stores the earlier
 * results to VU0 data memory with sqi, starting at address 0 ($vi3 = 0),
 * so vertex k ends up at quadword 4 + k (0-3 get stale registers). Read
 * with the VU's interlocks on VF registers, its 4-cycle clip-flag latency
 * (fcget sees the clipw issued 4 pairs earlier) and Q from the div of the
 * slot before, each vertex v gets:
 *     T = M * (v.xyz, 1)                     M = the mesh matrix (+0x58)
 *     Q = s.w / max(T.z, 1e-13)              s = scratchpad 0x30 (scale, w the projection distance)
 *     P = (T.xy * s.xy * Q + o.xy, T.z * s.z + o.z)    o = scratchpad 0x40 (offset, w the guard band)
 *     c1 = clip(P, 1), c2 = clip(P, |o.w|)   (bits 0-5: +x -x +y -y +z -z)
 *     code = (~(c1 << 6 | c2) & 0x3F0) | (c2 & 0xF)
 * i.e. bits 0-3: past a guard-band side, 4-5: inside the guard band in z,
 * 6-9: inside the screen in x/y. The clip variants store (T.xyz, code)
 * (the code by mfir.w, sign-extended) in the packet and VU0 memory; the
 * plain ones store T. The 1e-13 is the w of scratchpad 0x2C0, which the
 * clip variants set (0x29E12E13) and the microprograms use through mr32.
 *
 * Billboards (mode 0x20) take a 2-quadword record per 4 vertices (stride
 * 0x40): a centre c and a size s. With the screen-roll rows r0, r1
 * (scratchpad 0x300/0x310, xy):
 *     C = M * c (c.w included)
 *     A = r0 * s.x + r1 * s.y, B = r0 * s.x - r1 * s.y   (xy)
 *     corners C - A, C - B, C + A, C + B (xy), with C's z and w
 * and the clip variant projects each corner as above (one Q for all four)
 * and stores (corner.xy, C.z, code), and only the projected P.xy to VU0
 * memory (sqi.xy). The plain billboard variant writes no VU0 memory.
 *
 * Not reproduced: the pipeline's stale writes (VU0 memory 0-3, and for the
 * clip variant one group past the end) and the reads one or two groups
 * past the input. A vertex count of 0 makes the asm loop until it wraps;
 * here it transforms nothing. The kernels are written inline as plain
 * float C rather than through grx_vu0_callms (port/src/vu0_micro.c), so
 * no VU0 registers cross between stages; driving the asm's CPU side over
 * grx_vu0_callms gives the same packets and VU0 memory.
 */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

enum { R_A0 = 4, R_A2 = 6 };

#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))
#define SPR_F32(off) ((f32 *)(0x70000000 + (off)))
#define VU0_MEM ((GrxQuad *)0x11004000)

#define CLIP_TINY_BITS 0x29E12E13u /* 1e-13, scratchpad 0x2CC */

void GphVertXformClip(void);
void GphVertXform(void);
void GphVertXformClipBillboard(void);
void GphVertXformBillboard(void);

/* clipw.xyz p, w: bit 0 x > +|w|, 1 x < -|w|, 2/3 y, 4/5 z */
static u32 ClipFlags(const f32 *p, f32 w) {
    u32 c = 0;

    if (w < 0.0f) {
        w = -w;
    }
    if (p[0] > w) {
        c |= 1;
    }
    if (p[0] < -w) {
        c |= 2;
    }
    if (p[1] > w) {
        c |= 4;
    }
    if (p[1] < -w) {
        c |= 8;
    }
    if (p[2] > w) {
        c |= 0x10;
    }
    if (p[2] < -w) {
        c |= 0x20;
    }
    return c;
}

/* T = M * (v.xyz, w): mulax/madday/maddaz/maddw */
static void Xform(f32 *t, const f32 *m, const f32 *v, f32 w) {
    s32 i;

    for (i = 0; i < 4; i++) {
        t[i] = m[i] * v[0] + m[4 + i] * v[1] + m[8 + i] * v[2] + m[12 + i] * w;
    }
}

/* The projection and clip code of a view-space point (x, y from xy, z from
   zsrc): see the comment at the top. p gets P. */
static s32 ClipCode(f32 *p, f32 x, f32 y, f32 z) {
    const f32 *s = SPR_F32(0x30);
    const f32 *o = SPR_F32(0x40);
    f32 q;
    u32 c1, c2;

    q = VuDiv(s[3], z > VuBits(CLIP_TINY_BITS) ? z : VuBits(CLIP_TINY_BITS));
    p[0] = x * s[0] * q + o[0];
    p[1] = y * s[1] * q + o[1];
    p[2] = z * s[2] + o[2];
    c1 = ClipFlags(p, 1.0f);
    c2 = ClipFlags(p, o[3]);
    return (s32)((~(c1 << 6 | c2) & 0x3F0) | (c2 & 0xF));
}

/* The packet's tag and UNPACK, and its end in 0x700000F8 */
static void XformTag(u32 a2, u32 n) {
    *(u32 *)a2 = 0x10000001 + n;
    __builtin_memset((void *)(a2 + 0x10), 0, 16);
    *(u32 *)(a2 + 0x1C) = 0x6C000000 | (n << 16);
}

/* Vertices out, rounded up to 4 (the asm's andi keeps 16 bits) */
static u32 XformCount(u32 n) {
    return (n + 3) & 0xFFFC;
}

/* Transforms the vertex list (+0x60, count +0x48) by the mesh matrix,
   with clip codes in w (VU0 0x3C8/0x508) */
void GphVertXformClip(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = (u32)grx_gpr[R_A2].d[0];
    u32 n = mesh->vtxCount;
    u32 n4 = XformCount(n);
    const f32 *m = (const f32 *)mesh->mtx;
    const GrxQuad *in = (const GrxQuad *)mesh->verts;
    GrxQuad *out = (GrxQuad *)(a2 + 0x20);
    GrxQuad r;
    f32 p[4];
    u32 k;

    SPR_U32(0xF8) = a2 + 0x20 + n * 16;
    SPR_U32(0x2CC) = CLIP_TINY_BITS;
    for (k = 0; k < n4; k++) {
        Xform(r.f, m, in[k].f, 1.0f);
        r.i[3] = ClipCode(p, r.f[0], r.f[1], r.f[2]);
        out[k] = r;
        VU0_MEM[(4 + k) & 0xFF] = r;
    }
    XformTag(a2, n);
}

/* The same without clip codes (VU0 0x648/0x6C8), for GphMeshDrawVu0 */
void GphVertXform(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = (u32)grx_gpr[R_A2].d[0];
    u32 n = mesh->vtxCount;
    u32 n4 = XformCount(n);
    const f32 *m = (const f32 *)mesh->mtx;
    const GrxQuad *in = (const GrxQuad *)mesh->verts;
    GrxQuad *out = (GrxQuad *)(a2 + 0x20);
    GrxQuad r;
    u32 k;

    SPR_U32(0xF8) = a2 + 0x20 + n * 16;
    for (k = 0; k < n4; k++) {
        Xform(r.f, m, in[k].f, 1.0f);
        out[k] = r;
        VU0_MEM[(4 + k) & 0xFF] = r;
    }
    XformTag(a2, n);
}

/* The four corners of billboard record rec (centre, size) */
static void Billboard(GrxQuad *corner, const f32 *m, const GrxQuad *rec) {
    const f32 *r0 = SPR_F32(0x300);
    const f32 *r1 = SPR_F32(0x310);
    const f32 *s = rec[1].f;
    f32 c[4];
    f32 a[2], b[2];
    s32 i;

    Xform(c, m, rec[0].f, rec[0].f[3]);
    for (i = 0; i < 2; i++) {
        a[i] = r0[i] * s[0] + r1[i] * s[1];
        b[i] = r0[i] * s[0] - r1[i] * s[1];
    }
    for (i = 0; i < 4; i++) {
        corner[i].f[2] = c[2];
        corner[i].f[3] = c[3];
    }
    corner[0].f[0] = c[0] - a[0];
    corner[0].f[1] = c[1] - a[1];
    corner[1].f[0] = c[0] - b[0];
    corner[1].f[1] = c[1] - b[1];
    corner[2].f[0] = c[0] + a[0];
    corner[2].f[1] = c[1] + a[1];
    corner[3].f[0] = c[0] + b[0];
    corner[3].f[1] = c[1] + b[1];
}

/* Billboards (mode 0x20): each 0x40-byte record gives 4 corners, with
   clip codes (VU0 0x748/0x890) */
void GphVertXformClipBillboard(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = (u32)grx_gpr[R_A2].d[0];
    u32 n = mesh->vtxCount;
    u32 n4 = XformCount(n);
    const f32 *m = (const f32 *)mesh->mtx;
    const GrxQuad *in = (const GrxQuad *)mesh->verts;
    GrxQuad *out = (GrxQuad *)(a2 + 0x20);
    GrxQuad corner[4];
    f32 p[4];
    u32 k, i;

    SPR_U32(0xF8) = a2 + 0x20 + n * 16;
    SPR_U32(0x2CC) = CLIP_TINY_BITS;
    for (k = 0; k < n4; k += 4) {
        Billboard(corner, m, &in[k]);
        for (i = 0; i < 4; i++) {
            corner[i].i[3] = ClipCode(p, corner[i].f[0], corner[i].f[1], corner[i].f[2]);
            out[k + i] = corner[i];
            VU0_MEM[(4 + k + i) & 0xFF].f[0] = p[0];
            VU0_MEM[(4 + k + i) & 0xFF].f[1] = p[1];
        }
    }
    XformTag(a2, n);
}

/* The same without clip codes (VU0 0x9D8/0xA40), for GphMeshDrawVu0 */
void GphVertXformBillboard(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = (u32)grx_gpr[R_A2].d[0];
    u32 n = mesh->vtxCount;
    u32 n4 = XformCount(n);
    const f32 *m = (const f32 *)mesh->mtx;
    const GrxQuad *in = (const GrxQuad *)mesh->verts;
    GrxQuad *out = (GrxQuad *)(a2 + 0x20);
    u32 k;

    SPR_U32(0xF8) = a2 + 0x20 + n * 16;
    for (k = 0; k < n4; k += 4) {
        Billboard(&out[k], m, &in[k]);
    }
    XformTag(a2, n);
}
