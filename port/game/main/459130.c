/* Port-only C for the mesh pipeline's triangle stages, which the
   decompilation still has in assembly (asm/main/main/058D48.s):
   GphTriCullClip (0x00459130), GphTriCullClipNear (0x00459498),
   GphTriCull (0x004598F0), GphVertCompact (0x00459D48) and GphVertIndex
   (0x00459DA0). */
/*
 * Register contract (the drivers, port/game/main/4578B4.c ...): $a0 = the
 * mesh, $a2 = where the triangle packet goes (also scratchpad
 * 0x700000FC), $t9 = 0x70000000; the transformed vertices are at
 * (0x700000F4) + 0x20, 16 bytes each, w = the clip code (low halfword)
 * from GphVertXformClip* or T.w from GphVertXform*. Outputs, all in
 * memory: the triangle packet, the triangle count (0x700000E0), its
 * 16-aligned end (0x70000100), the queues of the clip variants
 * (0x70000028 gphClipTriIdx, 0x7000002C gphNearClipTriIdx, write
 * pointers), and the "used" mark (byte 0xE of a vertex = 0xFF) that
 * GphVertCompact reads. No caller reads the registers the asm leaves.
 *
 * The packet: a DMA CNT tag (qwc = (count + 1) / 2 + 1, first word
 * only), at +0x10 VIF codes (NOP, NOP, NOP, UNPACK V4-16 of count
 * quadwords to VU address 0x17F; the packet builders add FLUSHE/MSCNT),
 * then from +0x20 one 8-byte entry per kept triangle: the mesh's entry
 * (3 vertex byte offsets + material) with each halfword >> 4, and a zero
 * entry after the last (except GphTriCull's mode-2 copy).
 *
 * The facing tests are VU0 microprograms (port/docs/vu.md): with e1 = b - a,
 * e2 = c - a, d = ((n.x a.x + n.y a.y) + n.z a.z) for n = e1 x e2, i.e.
 * the triangle seen from the eye (view-space a), and the MAC flags of d:
 * 0xAA8/0xB08 (one triangle in $vf1-3 or $vf4-6, result in $vi2/$vi3)
 * give 0x80 when d is negative; 0xB68/0xC18 (two triangles each, the
 * vertices fetched from VU0 memory by index + 4, where GphVertXform put
 * them) give 0x80 for negative and 0x08 for zero. The CPU side pipelines
 * them one call ahead; written inline here, in order, as plain float C
 * (driving the asm's CPU side over grx_vu0_callms gives the same lists).
 *
 * Not reproduced: the clip variants load (and test) the entry after the
 * last triangle without using the result; reading it here could run past
 * the scratchpad, so it isn't. GphTriCull does use the second triangle
 * of its last pair, which is that entry when the count is odd: that is
 * kept. A triangle count of 0 makes GphTriCull's mode-2 copy loop until it
 * wraps; here it copies nothing.
 */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

enum { R_A0 = 4, R_A2 = 6 };

#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))
#define VU0_MEM ((GrxQuad *)0x11004000)

void GphTriCullClip(void);
void GphTriCullClipNear(void);
void GphTriCull(void);
void GphVertCompact(void);
void GphVertIndex(void);

/* d for the facing test (see the top) */
static f32 FacingDot(const f32 *a, const f32 *b, const f32 *c) {
    f32 e1[3], e2[3], n[3], p[3];
    s32 i;

    for (i = 0; i < 3; i++) {
        e1[i] = b[i] - a[i];
        e2[i] = c[i] - a[i];
    }
    /* opmula ACC, e1, e2; opmsub n, e2, e1 */
    n[0] = e1[1] * e2[2] - e2[1] * e1[2];
    n[1] = e1[2] * e2[0] - e2[2] * e1[0];
    n[2] = e1[0] * e2[1] - e2[0] * e1[1];
    for (i = 0; i < 3; i++) {
        p[i] = n[i] * a[i];
    }
    return (p[0] + p[1]) + p[2];
}

/* The MAC sign (0x80) and zero (0x08) flags of x */
static u32 MacFlagsX(f32 x) {
    VuWord w;
    u32 f = 0;

    w.f = x;
    if (w.u & 0x80000000) {
        f |= 0x80;
    }
    if (x == 0.0f) {
        f |= 0x08;
    }
    return f;
}

static u32 VertCode(u32 vbase, u16 off) {
    return *(u16 *)(vbase + off + 0xC);
}

static void MarkUsed(u32 vbase, const u16 *tri) {
    *(u8 *)(vbase + tri[0] + 0xE) = 0xFF;
    *(u8 *)(vbase + tri[1] + 0xE) = 0xFF;
    *(u8 *)(vbase + tri[2] + 0xE) = 0xFF;
}

/* psrlh 4 of an 8-byte entry */
static void PutIndices(u16 *dst, const u16 *src) {
    s32 i;

    for (i = 0; i < 4; i++) {
        dst[i] = src[i] >> 4;
    }
}

/* Appends the raw 8-byte entry to the queue whose write pointer is at
   scratchpad off */
static void Queue(u32 off, const u16 *tri) {
    u32 p = SPR_U32(off);

    __builtin_memcpy((void *)p, tri, 8);
    SPR_U32(off) = p + 8;
}

/* The packet's tag and UNPACK, its count (0x700000E0) and end (0x70000100) */
static void CullTag(u32 a2, u32 end, u32 count) {
    SPR_U32(0xE0) = count;
    *(u32 *)a2 = 0x10000001 + ((count + 1) >> 1);
    __builtin_memset((void *)(a2 + 0x10), 0, 16);
    *(u32 *)(a2 + 0x1C) = 0x6D00017F | (count << 16);
    SPR_U32(0x100) = ((end + 8) >> 4) << 4;
}

/* Both clip variants: a triangle whose ANDed codes (| the facing flag << 3)
   equal t4 (inside the screen and the guard band's z, not all past one
   guard-band side, facing the right way unless mode bit 1) goes to the
   packet; one that matches but for the screen bits (0x3C0) goes to
   gphClipTriIdx. With near set, a triangle not taken whose vertices are
   not all inside the guard band's -z side goes to gphNearClipTriIdx when
   some vertex is inside (ORed bit 5), else is dropped. */
static void TriCullClip(s32 near) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = (u32)grx_gpr[R_A2].d[0];
    u32 vbase = SPR_U32(0xF4) + 0x20;
    u32 mode = mesh->mode;
    u32 n = mesh->triCount;
    const u16 *tri = (const u16 *)mesh->indices;
    u16 *out = (u16 *)(a2 + 0x20);
    u32 count = 0;
    u32 t4;
    u32 i;

    if (mode & 2) {
        t4 = 0x3F0;
    } else {
        t4 = 0x7F0 ^ ((mode & 1) << 10);
    }
    for (i = 0; i < n; i++, tri += 4) {
        u32 c0 = VertCode(vbase, tri[0]);
        u32 c1 = VertCode(vbase, tri[1]);
        u32 c2 = VertCode(vbase, tri[2]);
        u32 s = c0 & c1 & c2;

        if (!(mode & 2)) {
            f32 d = FacingDot((f32 *)(vbase + tri[0]), (f32 *)(vbase + tri[1]), (f32 *)(vbase + tri[2]));

            s |= (MacFlagsX(d) & 0x80) << 3;
        }
        if (s == t4) {
            MarkUsed(vbase, tri);
            PutIndices(out, tri);
            out += 4;
            count++;
        } else if (!near) {
            if ((s | 0x3C0) == t4) {
                Queue(0x28, tri);
            }
        } else if ((c0 | c1 | c2) & 0x20) {
            if (!(s & 0x20)) {
                Queue(0x2C, tri);
            } else if ((s | 0x3C0) == t4) {
                Queue(0x28, tri);
            }
        }
    }
    __builtin_memset(out, 0, 8);
    CullTag(a2, (u32)out, count);
}

/* VU0 0xAA8/0xB08 per triangle: visible ones to the VU1 index list,
   ones crossing a screen edge to gphClipTriIdx */
void GphTriCullClip(void) {
    TriCullClip(0);
}

/* As GphTriCullClip, plus triangles crossing the near plane to
   gphNearClipTriIdx */
void GphTriCullClipNear(void) {
    TriCullClip(1);
}

/* VU0 0xB68/0xC18: back-face culling with the vertices GphVertXform left
   in VU0 memory, no clipping. Facing = the MAC flags of d equal to 0x80
   (mode bit 0 clear) or 0 (set): d == 0 is never kept. Mode bit 1: no
   culling, every entry copied (in blocks of 8, rounded up). */
void GphTriCull(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 a2 = (u32)grx_gpr[R_A2].d[0];
    u32 vbase = SPR_U32(0xF4) + 0x20;
    u32 mode = mesh->mode;
    u32 n = mesh->triCount;
    const u16 *tri = (const u16 *)mesh->indices;
    u16 *out = (u16 *)(a2 + 0x20);
    u32 count = 0;
    u32 t4;
    u32 i, k;

    if (mode & 2) {
        u32 blocks = ((n * 8 + 0x38) & 0xFFC0) / 64;

        for (i = 0; i < blocks * 32; i++) {
            out[i] = tri[i] >> 4;
        }
        out = (u16 *)(a2 + 0x20 + n * 8);
        count = n;
        SPR_U32(0xE0) = count;
        *(u32 *)a2 = 0x10000001 + ((count + 1) >> 1);
        __builtin_memset((void *)(a2 + 0x10), 0, 16);
        *(u32 *)(a2 + 0x1C) = 0x6D00017F | (count << 16);
        SPR_U32(0x100) = (((u32)out + 8) >> 4) << 4;
        return;
    }
    t4 = 0x80 ^ ((mode & 1) << 7);
    /* Pairs of triangles; the last pair's second one is read past the end
       when the count is odd, as on the EE */
    for (i = 0; i < ((n + 1) >> 1) * 2; i++, tri += 4) {
        const f32 *v[3];

        for (k = 0; k < 3; k++) {
            v[k] = VU0_MEM[(((tri[k] >> 4) & 0x7F) + 4) & 0xFF].f;
        }
        if (MacFlagsX(FacingDot(v[0], v[1], v[2])) == t4) {
            PutIndices(out, tri);
            out += 4;
            count++;
            MarkUsed(vbase, tri);
        }
    }
    __builtin_memset(out, 0, 8);
    CullTag(a2, (u32)out, count);
}

/* Renumbers the vertices marked used (byte 0xE = 0xFF): the w words of the
   last `used` vertex slots get their indices, ascending, and the used
   count goes to 0x700000E4 */
void GphVertCompact(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 n = mesh->vtxCount;
    u32 vbase = SPR_U32(0xF4) + 0x20;
    u32 slot = n;
    s32 i;

    for (i = (s32)n - 1; i >= 0; i--) {
        if (*(u8 *)(vbase + i * 16 + 0xE) == 0xFF) {
            slot--;
            *(u32 *)(vbase + slot * 16 + 0xC) = (u32)i;
        }
    }
    SPR_U32(0xE4) = n - slot;
}

/* Header flag 2 (no culling): every vertex numbered in order; otherwise
   GphVertCompact */
void GphVertIndex(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 n;
    u32 base;
    u32 i;

    if (!(mesh->mode & 2)) {
        GphVertCompact();
        return;
    }
    n = mesh->vtxCount;
    base = SPR_U32(0xF4);
    for (i = 0; i < n; i++) {
        *(u32 *)(base + 0x2C + i * 16) = i;
    }
    SPR_U32(0xE4) = n;
}
