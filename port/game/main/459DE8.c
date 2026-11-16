/* Port-only C for GphClipTriGather (0x00459DE8) and GphNearClipTris
   (0x00459FF4), which the decompilation still has in assembly
   (asm/main/main/058D48.s). */
/*
 * Register contract (GphMeshDrawClip, port/game/main/4578B4.c): $a0 = the
 * mesh, $t9 = 0x70000000. They read the queues GphTriCullClip(Near) filled
 * (gphClipTriIdx up to scratchpad 0x70000028, gphNearClipTriIdx up to
 * 0x7000002C: 8-byte entries of 3 vertex byte offsets into the
 * transformed vertices at (0x700000F4) + 0x20, and a material offset), and
 * write view-space vertices to gphClipVerts (0x40 each: position with the
 * clip code in w, normal, UV, colour, as floats) and 16-byte triangles to
 * gphClipTris (3 vertex pointers, then the material: mesh +0x54 + 0x10 +
 * the entry's offset), with the write pointers in 0x70000024 / 0x70000020.
 * GphClipTriDraw takes it from there.
 *
 * Both are VU0 macro code, done here as float C on the port's register
 * file grx_vf[], since the asm stores some registers it doesn't fully
 * write (the normals when the mesh has none, lanes the interpolation
 * leaves). The unpacking matches the MMI sequences: normals 4 x s16 / 4096
 * (all four), UVs 2 x s16 * 4 / 4096 with z = w = (the word's sign) * 4 /
 * 4096, colours 4 x u8 read as signed bytes (psraw 24), each array past
 * its 16-byte UNPACK header. The colour integers are staged at scratchpad
 * 0x700002C0-0x700002EF as in the asm.
 */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

enum { R_A0 = 4 };

#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))
#define VF(n) (grx_vf[n].f)

extern u8 gphClipVerts[0x4000];
extern u8 gphClipTris[0x800];
extern u8 gphNearClipTriIdx[0x200];
extern u8 gphClipTriIdx[0x200];

void GphClipTriGather(void);
void GphNearClipTris(void);

/* The mesh's arrays, each past its UNPACK header (normals 0 when none) */
typedef struct {
    u32 vbase;
    u32 nrm;
    u32 uv;
    u32 col;
    u32 mat;
} ClipSrc;

static void GetSrc(ClipSrc *s, GphGaObject *mesh) {
    s->vbase = SPR_U32(0xF4) + 0x20;
    s->nrm = (u32)mesh->normals;
    if (s->nrm != 0) {
        s->nrm += 0x10;
    }
    s->uv = (u32)mesh->uvs + 0x10;
    s->col = (u32)mesh->colors + 0x10;
    s->mat = (u32)mesh->gsPkt + 0x10;
}

/* Vertex at byte offset off (index * 16) into slot j: position to $vf5+j,
   normal to $vf10+j (when the mesh has normals), UV to $vf15+j, colour to
   scratchpad 0x2C0 + 16j and, as floats, $vf20+j */
static void LoadVert(const ClipSrc *s, u32 off, s32 j) {
    s32 *ci = (s32 *)(0x700002C0 + j * 16);
    s32 i;

    __builtin_memcpy(VF(5 + j), (void *)(s->vbase + off), 16);
    if (s->nrm != 0) {
        const s16 *h = (const s16 *)(s->nrm + (off >> 1));

        for (i = 0; i < 4; i++) {
            VF(10 + j)[i] = VuItof(h[i], 12);
        }
    }
    {
        s32 w = *(s32 *)(s->uv + (off >> 2));
        s32 ext = w < 0 ? -4 : 0;

        VF(15 + j)[0] = VuItof((s16)w * 4, 12);
        VF(15 + j)[1] = VuItof((s16)(w >> 16) * 4, 12);
        VF(15 + j)[2] = VuItof(ext, 12);
        VF(15 + j)[3] = VuItof(ext, 12);
    }
    {
        u32 c = *(u32 *)(s->col + (off >> 2));

        for (i = 0; i < 4; i++) {
            ci[i] = (s8)(c >> (i * 8));
        }
    }
}

static void ColoursToFloat(void) {
    s32 j, i;

    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            VF(20 + j)[i] = (f32)((s32 *)(0x700002C0 + j * 16))[i];
        }
    }
}

/* The three vertices of entry tri, rotated by rot: slot j gets vertex
   (rot + j) % 3 */
static void LoadTri(const ClipSrc *s, const u16 *tri, s32 rot) {
    s32 j;

    for (j = 0; j < 3; j++) {
        LoadVert(s, tri[(rot + j) % 3], j);
    }
    ColoursToFloat();
}

/* One gphClipVerts record from the registers of a vertex */
static void PutVert(u32 dst, s32 pos, s32 nrm, s32 uv, s32 col) {
    __builtin_memcpy((void *)dst, VF(pos), 16);
    __builtin_memcpy((void *)(dst + 0x10), VF(nrm), 16);
    __builtin_memcpy((void *)(dst + 0x20), VF(uv), 16);
    __builtin_memcpy((void *)(dst + 0x30), VF(col), 16);
}

static void PutTri(u32 dst, u32 a, u32 b, u32 c, u32 mat) {
    u32 *t = (u32 *)dst;

    t[0] = a;
    t[1] = b;
    t[2] = c;
    t[3] = mat;
}

/* The loaded triangle as it is: 3 records and one triangle */
static void PutLoadedTri(u32 *t5, u32 *t6, u32 mat) {
    s32 j;

    for (j = 0; j < 3; j++) {
        PutVert(*t5 + j * 0x40, 5 + j, 10 + j, 15 + j, 20 + j);
    }
    PutTri(*t6, *t5, *t5 + 0x40, *t5 + 0x80, mat);
    *t5 += 0xC0;
    *t6 += 0x10;
}

/* For each gphClipTriIdx triangle copies position/normal/UV/colour of its
   3 vertices to gphClipVerts and a pointer entry to gphClipTris */
void GphClipTriGather(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 end = SPR_U32(0x28);
    const u16 *tri = (const u16 *)gphClipTriIdx;
    u32 t5 = SPR_U32(0x24);
    u32 t6 = SPR_U32(0x20);
    ClipSrc s;

    GetSrc(&s, mesh);
    do {
        LoadTri(&s, tri, 0);
        PutLoadedTri(&t5, &t6, s.mat + tri[3]);
        tri += 4;
    } while ((u32)tri != end);
    SPR_U32(0x24) = t5;
    SPR_U32(0x20) = t6;
}

/* $vfN.xyz normalised (vrsqrt), with $vf(tmp) as the asm's scratch */
static void Normalize(s32 n, s32 tmp) {
    f32 *v = VF(n);
    f32 *t = VF(tmp);
    f32 q;
    s32 i;

    for (i = 0; i < 3; i++) {
        t[i] = v[i] * v[i];
    }
    t[0] = t[0] + t[1];
    t[0] = t[0] + t[2];
    q = VuRsqrt(t[0]);
    for (i = 0; i < 3; i++) {
        v[i] = v[i] * q;
    }
}

/* d = base + sign * diff * t on lanes 0..n-1 */
static void Lerp(s32 d, s32 base, s32 diff, f32 t, s32 n, s32 sign) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (sign > 0) {
            VF(d)[i] = VF(base)[i] + VF(diff)[i] * t;
        } else {
            VF(d)[i] = VF(base)[i] - VF(diff)[i] * t;
        }
    }
}

static void Diff(s32 d, s32 a, s32 b, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        VF(d)[i] = VF(a)[i] - VF(b)[i];
    }
}

/* Slots 0 and 1 in front, slot 2 behind the near plane ($vf1.z): the
   points where its edges to the other two cross it ($vf8/$vf9 ...) make a
   quad, sent as two triangles (0, 1, n0) and (1, n1, n0) */
static void NearSplitQuad(u32 *t5, u32 *t6, u32 mat, s32 haveNrm) {
    f32 *v2 = VF(2);

    v2[2] = VF(1)[2] - VF(7)[2];
    Diff(3, 5, 7, 3);
    Diff(4, 6, 7, 3);
    v2[0] = VuDiv(v2[2], VF(3)[2]);
    v2[1] = VuDiv(v2[2], VF(4)[2]);
    Lerp(8, 7, 3, v2[0], 3, 1);
    Lerp(9, 7, 4, v2[1], 3, 1);
    if (haveNrm) {
        Diff(3, 10, 12, 3);
        Diff(4, 11, 12, 3);
        Lerp(13, 12, 3, v2[0], 3, 1);
        Lerp(14, 12, 4, v2[1], 3, 1);
        Normalize(13, 3);
        Normalize(14, 4);
    }
    Diff(3, 15, 17, 2);
    Diff(4, 16, 17, 2);
    Lerp(18, 17, 3, v2[0], 2, 1);
    Lerp(19, 17, 4, v2[1], 2, 1);
    Diff(3, 20, 22, 4);
    Diff(4, 21, 22, 4);
    Lerp(23, 22, 3, v2[0], 4, 1);
    Lerp(24, 22, 4, v2[1], 4, 1);
    PutVert(*t5, 5, 10, 15, 20);
    PutVert(*t5 + 0x40, 6, 11, 16, 21);
    PutVert(*t5 + 0x80, 8, 13, 18, 23);
    PutVert(*t5 + 0xC0, 9, 14, 19, 24);
    PutTri(*t6, *t5, *t5 + 0x40, *t5 + 0x80, mat);
    PutTri(*t6 + 0x10, *t5 + 0x40, *t5 + 0xC0, *t5 + 0x80, mat);
    *t5 += 0x100;
    *t6 += 0x20;
}

/* Slot 0 in front, slots 1 and 2 behind: the triangle cut down to
   (0, n0, n1) */
static void NearSplitTri(u32 *t5, u32 *t6, u32 mat, s32 haveNrm) {
    f32 *v2 = VF(2);

    v2[2] = VF(5)[2] - VF(1)[2];
    Diff(3, 5, 6, 3);
    Diff(4, 5, 7, 3);
    v2[0] = VuDiv(v2[2], VF(3)[2]);
    v2[1] = VuDiv(v2[2], VF(4)[2]);
    Lerp(8, 5, 3, v2[0], 3, -1);
    Lerp(9, 5, 4, v2[1], 3, -1);
    if (haveNrm) {
        Diff(3, 10, 11, 3);
        Diff(4, 10, 12, 3);
        Lerp(13, 10, 3, v2[0], 3, -1);
        Lerp(14, 10, 4, v2[1], 3, -1);
        Normalize(13, 3);
        Normalize(14, 4);
    }
    Diff(3, 15, 16, 2);
    Diff(4, 15, 17, 2);
    Lerp(18, 15, 3, v2[0], 2, -1);
    Lerp(19, 15, 4, v2[1], 2, -1);
    Diff(3, 20, 21, 4);
    Diff(4, 20, 22, 4);
    Lerp(23, 20, 3, v2[0], 4, -1);
    Lerp(24, 20, 4, v2[1], 4, -1);
    PutVert(*t5, 5, 10, 15, 20);
    PutVert(*t5 + 0x40, 8, 13, 18, 23);
    PutVert(*t5 + 0x80, 9, 14, 19, 24);
    PutTri(*t6, *t5, *t5 + 0x40, *t5 + 0x80, mat);
    *t5 += 0xC0;
    *t6 += 0x10;
}

/* The facing test of VU0 0xAA8/0xB08 (see 459130.c): 0x80 when
   ((n.x a.x + n.y a.y) + n.z a.z) is negative, n = (b - a) x (c - a) */
static u32 FacingSign(const f32 *a, const f32 *b, const f32 *c) {
    f32 e1[3], e2[3], n[3], p[3];
    VuWord d;
    s32 i;

    for (i = 0; i < 3; i++) {
        e1[i] = b[i] - a[i];
        e2[i] = c[i] - a[i];
    }
    n[0] = e1[1] * e2[2] - e2[1] * e1[2];
    n[1] = e1[2] * e2[0] - e2[2] * e1[0];
    n[2] = e1[0] * e2[1] - e2[0] * e1[1];
    for (i = 0; i < 3; i++) {
        p[i] = n[i] * a[i];
    }
    d.f = (p[0] + p[1]) + p[2];
    return (d.u & 0x80000000) ? 0x80 : 0;
}

/* Splits the gphNearClipTriIdx triangles at the near plane (clip block
   +0x28, view z): those wholly in front are copied, the ones crossing it
   cut (a quad as two triangles, or a smaller triangle), the ones wholly
   behind dropped. The lists restart at gphClipVerts/gphClipTris. Unless
   mode bit 1, the new triangles are then culled by facing (VU0
   0xAA8/0xB08; kept when the sign flag is 0x80 with mode bit 0 clear, 0
   with it set), in place; the asm puts a sentinel entry (four 0x70000000)
   after them for its one-ahead pipeline, which is written here too. */
void GphNearClipTris(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 end = SPR_U32(0x2C);
    const u16 *tri = (const u16 *)gphNearClipTriIdx;
    u32 t5 = (u32)gphClipVerts;
    u32 t6 = (u32)gphClipTris;
    u8 *clip = *(u8 **)0x700000EC;
    f32 near;
    s32 haveNrm;
    ClipSrc s;
    u32 mode;
    u32 a2, v1, e, t8;

    GetSrc(&s, mesh);
    haveNrm = s.nrm != 0;
    __builtin_memcpy((void *)0x700002C8, clip + 0x28, 4);
    __builtin_memcpy(VF(1), (void *)0x700002C0, 16);
    __builtin_memcpy(&near, clip + 0x28, 4);
    do {
        s32 b0 = *(f32 *)(s.vbase + tri[0] + 8) <= near;
        s32 b1 = *(f32 *)(s.vbase + tri[1] + 8) <= near;
        s32 b2 = *(f32 *)(s.vbase + tri[2] + 8) <= near;
        s32 behind = b0 + b1 + b2;
        u32 mat = s.mat + tri[3];

        if (behind == 0) {
            LoadTri(&s, tri, 0);
            PutLoadedTri(&t5, &t6, mat);
        } else if (behind == 1) {
            /* the one behind goes to slot 2 */
            LoadTri(&s, tri, b2 ? 0 : b1 ? 2 : 1);
            NearSplitQuad(&t5, &t6, mat, haveNrm);
        } else if (behind == 2) {
            /* the one in front goes to slot 0 */
            LoadTri(&s, tri, !b0 ? 0 : !b1 ? 1 : 2);
            NearSplitTri(&t5, &t6, mat, haveNrm);
        }
        tri += 4;
    } while ((u32)tri != end);

    SPR_U32(0x24) = t5;
    if (t6 == (u32)gphClipTris) {
        SPR_U32(0x24) = (u32)gphClipVerts;
        SPR_U32(0x20) = (u32)gphClipTris;
        return;
    }
    a2 = t6;
    mode = mesh->mode;
    if (mode & 2) {
        SPR_U32(0x20) = a2;
        return;
    }
    PutTri(a2, 0x70000000, 0x70000000, 0x70000000, 0x70000000);
    t8 = 0x80 ^ ((mode & 1) << 7);
    v1 = (u32)gphClipTris;
    for (e = (u32)gphClipTris; e != a2; e += 0x10) {
        const u32 *t = (const u32 *)e;

        if (FacingSign((f32 *)t[0], (f32 *)t[1], (f32 *)t[2]) == t8) {
            __builtin_memmove((void *)v1, (void *)e, 16);
            v1 += 0x10;
        }
    }
    SPR_U32(0x20) = v1;
}
