/* Port-only C for GphClipVertProject (0x0045B964), which the decompilation
   still has in assembly (asm/main/main/058D48.s). Its only caller is
   GphClipTriDraw (port/game/main/45AC74.c). */
/*
 * Takes the view-space vertices in gphClipVerts (up to scratchpad
 * 0x70000024; position, normal, UV, colour) and makes gphClipScreenVerts
 * (0x40 each, the same index): +0x00 XYZF (float), +0x10 STQ, +0x20 RGBA
 * (float, clamped to 0-255), +0x30 the clip-space x/y used against the
 * screen edges, with its outcode (bits 0-3, clipw against scratchpad
 * 0x4C) at +0x3C. Then gphClipScreenTris: gphClipTris (up to 0x70000020)
 * with the vertex pointers moved to gphClipScreenVerts, its end to
 * 0x70000018; the vertex end goes to 0x7000001C.
 *
 * The maths is VU1's projection and lighting (port/docs/vu.md, "The
 * phases") in VU0 macro code, done here as float C on the port's VU0
 * register file grx_vf[], since the asm stores lanes it doesn't write
 * (STQ.w comes from whatever $vf6.w held) and leaves its registers
 * changed. Inputs: $a0 the mesh, $a1 the mode byte (bit 3: lit), the
 * header quadwords at scratchpad 0x60/0x70/0x80/0xB0/0xC0 (projection
 * scale, offset, counts with the projection distance in w, fog, UV
 * offset), 0x30/0x40 (clip scale and offset, w the clip bound), the
 * ambient colour (mesh +0x38) and for lit meshes the light block (+0x3C)
 * and light matrix (+0x5C). Like the packet builders it first stores
 * mesh +0x70 to 0x78 and mesh +0x78 to 0xC0.
 *
 * Per vertex (p position, u UV, c colour; s, o, d the scale, offset and
 * distance; f the fog quadword):
 *     Q = d / p.z
 *     XYZF = (o.xy + s.xy * p.xy * Q, o.z + s.z * Q,
 *             16 * (fog bias + f.x) + f.y / (f.w + |p|^2))
 *     STQ = ((u + UV offset) * Q, Q)
 *     RGBA = clamp(c + ambient, 0, f.z)       unlit
 *     clip = p.xy * clip scale * Q + clip offset
 * Lit: the colour plus 4 directional lights (colour * max(0, N.L) *
 * intensity) and 4 point lights (colour * max(0, (N.d) (1 - |d|/range))
 * with d scaled by 1/range), N = the light matrix * the normal, plus the
 * ambient; with a specular factor (mesh +0x30) the alpha becomes
 * -factor * I0 * max(0, -(R.L0))^4, R the reflection of the view
 * direction about N.
 */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

enum { R_A0 = 4, R_A1 = 5 };

#define SPR_U32(off) (*(u32 *)(0x70000000 + (off)))
#define SPR_F32(off) ((f32 *)(0x70000000 + (off)))
#define VF(n) (grx_vf[n].f)
#define ACC (grx_vf[GRX_ACC].f)

extern u8 gphClipVerts[0x4000];
extern u8 gphClipTris[0x800];
extern u8 gphClipScreenTris[0x4000];
extern u8 gphClipScreenVerts[0x20000];

void GphClipVertProject(void);

static void Load(s32 vf, u32 addr) {
    __builtin_memcpy(VF(vf), (void *)addr, 16);
}

static void Store(u32 addr, s32 vf) {
    __builtin_memcpy((void *)addr, VF(vf), 16);
}

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

/* The set-up both paths share: the header words, $vf16-$vf18, $vf20
   (ambient), $vf29/$vf30 (fog, UV offset), $vf31 = 0 and $vf17.w = 16 *
   (fog bias + fog2) */
static void Setup(GphGaObject *mesh) {
    s32 i;

    __builtin_memcpy((void *)0x70000078, &mesh->unk70, 8);
    __builtin_memcpy((void *)0x700000C0, &mesh->uOffset, 8);
    Load(20, (u32)mesh->ambient);
    Load(29, 0x700000B0);
    Load(30, 0x700000C0);
    Load(16, 0x70000060);
    Load(17, 0x70000070);
    Load(18, 0x70000080);
    for (i = 0; i < 4; i++) {
        VF(31)[i] = VF(31)[i] * 0.0f;
    }
    VF(17)[3] = VF(17)[3] + VF(29)[0];
    for (i = 0; i < 4; i++) {
        VF(17)[3] = VF(17)[3] + VF(17)[3];
    }
}

/* The projection of the vertex at v into the screen vertex at dst, with
   the colour already in $vf7 (see the top) */
static void Project(u32 v, u32 dst) {
    f32 *v1 = VF(1), *v2 = VF(2), *v3 = VF(3), *v4 = VF(4);
    f32 *v5 = VF(5), *v6 = VF(6), *v7 = VF(7);
    f32 q;
    s32 i;

    Load(1, v);
    Load(2, v + 0x20);
    v2[2] = 1.0f;
    for (i = 0; i < 4; i++) {
        v7[i] = v7[i] > 0.0f ? v7[i] : 0.0f;
        v7[i] = v7[i] < VF(29)[2] ? v7[i] : VF(29)[2];
    }
    v2[0] = v2[0] + VF(30)[0];
    v2[1] = v2[1] + VF(30)[1];
    for (i = 0; i < 3; i++) {
        v4[i] = v1[i] * v1[i];
    }
    ACC[3] = VF(29)[3] + v4[0];
    ACC[3] = ACC[3] + v4[1];
    v4[3] = ACC[3] + v4[2];
    q = VuDiv(VF(18)[3], v1[2]);
    v1[2] = 1.0f;
    for (i = 0; i < 4; i++) {
        v5[i] = VF(16)[i] * v1[i];
    }
    for (i = 0; i < 3; i++) {
        ACC[i] = VF(17)[i] + VF(31)[i];
        v6[i] = v2[i] * q;
        v5[i] = ACC[i] + v5[i] * q;
    }
    q = VuDiv(VF(29)[1], v4[3]);
    v5[3] = VF(17)[3] + q;
    Load(3, v);
    Load(1, 0x70000030);
    Load(2, 0x70000040);
    for (i = 0; i < 2; i++) {
        v3[i] = v3[i] * v1[i];
        v3[i] = v3[i] * v6[2];
        v3[i] = v3[i] + v2[i];
    }
    Store(dst, 5);
    Store(dst + 0x10, 6);
    Store(dst + 0x20, 7);
    Store(dst + 0x30, 3);
    *(u32 *)(dst + 0x3C) = ClipFlags(v3, v2[3]) & 0xF;
}

/* Lights the vertex at v into $vf7 (lb: the light block's 16 quadwords,
   ltm: the light matrix) */
static void Light(GphGaObject *mesh, u32 v, u32 lb, u32 ltm) {
    f32 *v1 = VF(1), *v2 = VF(2), *v3 = VF(3), *v4 = VF(4), *v5 = VF(5), *v6 = VF(6), *v7 = VF(7);
    f32 *v9 = VF(9), *v10 = VF(10), *v11 = VF(11), *v12 = VF(12);
    f32 *v14 = VF(14), *v15 = VF(15), *v25 = VF(25), *v26 = VF(26), *v27 = VF(27);
    s32 spec;
    s32 i;

    Load(1, v + 0x10);
    Load(2, ltm);
    Load(3, ltm + 0x10);
    Load(4, ltm + 0x20);
    Load(14, v);
    Load(25, lb + 0x40);
    Load(26, lb + 0x50);
    Load(27, lb + 0x60);
    /* N = the light matrix * the normal */
    for (i = 0; i < 3; i++) {
        v15[i] = v2[i] * v1[0] + v3[i] * v1[1] + v4[i] * v1[2];
    }
    /* point lights: (light - p) / range */
    for (i = 0; i < 4; i++) {
        v25[i] = v25[i] - v14[0];
        v26[i] = v26[i] - v14[1];
        v27[i] = v27[i] - v14[2];
    }
    for (i = 0; i < 4; i++) {
        v1[i] = VF(24)[i] * v15[0];
        v2[i] = VF(24)[i] * v15[1];
        v3[i] = VF(24)[i] * v15[2];
        v4[i] = VF(31)[i] + v15[0];
        v5[i] = VF(31)[i] + v15[1];
        v6[i] = VF(31)[i] + v15[2];
    }
    for (i = 0; i < 4; i++) {
        v25[i] = v25[i] * VF(28)[i];
        v26[i] = v26[i] * VF(28)[i];
        v27[i] = v27[i] * VF(28)[i];
        v7[i] = v25[i] * v25[i] + v26[i] * v26[i] + v27[i] * v27[i];
    }
    for (i = 0; i < 4; i++) {
        v12[i] = VF(31)[i] + VuRsqrt(v7[i]);
    }
    for (i = 0; i < 4; i++) {
        v9[i] = v1[i] * VF(21)[i] + v2[i] * VF(22)[i] + v3[i] * VF(23)[i];
        v10[i] = v4[i] * v25[i] + v5[i] * v26[i] + v6[i] * v27[i];
        v9[i] = v9[i] > VF(31)[i] ? v9[i] : VF(31)[i];
        v10[i] = v10[i] > VF(31)[i] ? v10[i] : VF(31)[i];
    }
    Load(11, v + 0x30);
    for (i = 0; i < 4; i++) {
        ACC[i] = VF(31)[i] - v10[i];
        v10[i] = ACC[i] + v10[i] * v12[i];
        v10[i] = v10[i] > VF(31)[3] ? v10[i] : VF(31)[3];
    }
    for (i = 0; i < 8; i++) {
        Load(1 + i, lb + 0x80 + i * 16);
    }
    for (i = 0; i < 4; i++) {
        f32 a = v11[i] + VF(20)[i];

        a = a + VF(1)[i] * v9[0];
        a = a + VF(2)[i] * v9[1];
        a = a + VF(3)[i] * v9[2];
        a = a + VF(4)[i] * v9[3];
        a = a + VF(5)[i] * v10[0];
        a = a + VF(6)[i] * v10[1];
        a = a + VF(7)[i] * v10[2];
        ACC[i] = a;
        v7[i] = a + VF(8)[i] * v10[3];
    }
    __builtin_memcpy(&spec, &mesh->unk30, 4);
    if (spec != 0) {
        f32 q;

        v2[0] = 0.0f + 1.0f;
        grx_vf[3].u[0] = 0xC0000000; /* qmtc2 of lui 0xC000: -2.0, the high word all ones */
        grx_vf[3].u[1] = 0xFFFFFFFF;
        v4[0] = 0.0f - VF(24)[0];
        v4[3] = VF(30)[3] * v4[0];
        ACC[0] = 0.0f - VF(21)[0];
        ACC[1] = 0.0f - VF(22)[0];
        ACC[2] = 0.0f - VF(23)[0];
        for (i = 0; i < 3; i++) {
            v5[i] = ACC[i] + 0.0f * 0.0f;
            v1[i] = v14[i] * v14[i];
        }
        v1[0] = v2[0] * v1[0] + v2[0] * v1[1] + v2[0] * v1[2];
        q = VuRsqrt(v1[0]);
        for (i = 0; i < 3; i++) {
            v14[i] = v14[i] * q;
            v1[i] = v14[i] * v15[i];
        }
        v1[0] = v3[0] * v1[0] + v3[0] * v1[1] + v3[0] * v1[2];
        for (i = 0; i < 3; i++) {
            ACC[i] = v14[i] + 0.0f;
            v15[i] = ACC[i] + v15[i] * v1[0];
        }
        for (i = 0; i < 3; i++) {
            v15[i] = v15[i] * v5[i];
        }
        v15[0] = v2[0] * v15[0] + v2[0] * v15[1] + v2[0] * v15[2];
        v15[0] = v15[0] > 0.0f ? v15[0] : 0.0f;
        v15[0] = v15[0] * v15[0];
        v15[0] = v15[0] * v15[0];
        v7[3] = v4[3] * v15[0];
    }
}

/* Lights (unlit: colour + ambient; lit with the light block) and projects
   gphClipVerts into gphClipScreenVerts with outcodes, then builds
   gphClipScreenTris */
void GphClipVertProject(void) {
    GphGaObject *mesh = (GphGaObject *)(u32)grx_gpr[R_A0].d[0];
    u32 lit = (u32)grx_gpr[R_A1].d[0] & 8;
    u32 v = (u32)gphClipVerts;
    u32 end = SPR_U32(0x24);
    u32 dst = (u32)gphClipScreenVerts;
    u32 shift = (u32)gphClipScreenVerts - (u32)gphClipVerts;
    u32 t, o;
    s32 i;

    Setup(mesh);
    if (lit) {
        u32 lb = (u32)mesh->lightBlock + 0x10;

        Load(21, lb);
        Load(22, lb + 0x10);
        Load(23, lb + 0x20);
        Load(24, lb + 0x30);
        Load(28, lb + 0x70);
        Load(1, (u32)&mesh->unk30);
        VF(30)[3] = 1.0f * VF(1)[0];
        do {
            Light(mesh, v, lb, (u32)mesh->mtx2);
            Project(v, dst);
            v += 0x40;
            dst += 0x40;
        } while (v != end);
    } else {
        do {
            Load(11, v + 0x30);
            for (i = 0; i < 4; i++) {
                VF(7)[i] = VF(11)[i] + VF(20)[i];
            }
            Project(v, dst);
            v += 0x40;
            dst += 0x40;
        } while (v != end);
    }
    SPR_U32(0x1C) = dst;
    t = (u32)gphClipTris;
    o = (u32)gphClipScreenTris;
    end = SPR_U32(0x20);
    do {
        u32 *src = (u32 *)t;
        u32 *d = (u32 *)o;

        d[0] = src[0] + shift;
        d[1] = src[1] + shift;
        d[2] = src[2] + shift;
        d[3] = src[3];
        t += 0x10;
        o += 0x10;
    } while (t != end);
    SPR_U32(0x18) = o;
}
