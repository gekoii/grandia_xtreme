/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
    f32 rad;
    u8 pad24[0x44 - 0x24];
    s32 count;
    u8 pad48[0x50 - 0x48];
    s16 *idx;
    u8 pad54[0x60 - 0x54];
    u8 *verts;
    u8 pad64[0x80 - 0x64];
} Part;

typedef struct {
    u8 pad0[0xC];
    FVECTOR *mat;
    u8 pad10[0x40 - 0x10];
    Part *parts;
    u8 pad44[0x5E - 0x44];
    u16 nparts;
} Model;

extern s32 mapPolyRejectBound;
extern s32 mapPolyRejectPlane;
extern s32 mapPolyRejectInside;
extern s32 mapPolyAcceptCount;
extern s32 mapPolyMeshCount;
extern s32 mapPolyMeshNearCount;
extern FVECTOR mapPolyTmpTri[3];

s32 VuSphereTriBoundHit(f32 *, f32, FVECTOR *);
s32 VuTriPlaneNear(f32 *, f32, FVECTOR *);
s32 VuPointInTriangle(f32 *, f32, FVECTOR *);
f32 VuVecDot(f32 *, f32 *);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VAPPLY(o, m, v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(m), "r"(v) : "memory")

#define VSUB(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyzw $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

s32 MapPolyGatherPart(Model *m, f32 *pos, f32 *size, f32 r)
{
    FVECTOR v[2];
    FVECTOR *tri = mapPolyTmpTri;
    FVECTOR *mat = m->mat;
    Part *p = m->parts;
    f32 *sp = (f32 *)0x70003010;
    s32 i;
    s32 j;
    s16 *ip;
    u8 *verts;
    f32 lim;
    f32 d;

    copyq(v[0], pos);
    lim = __builtin_fabsf(size[1]) + 50.0f;
    mapPolyMeshCount += m->nparts;
    for (i = 0; i < m->nparts; p++, i++) {
        copyq(sp, p->pos);
        ((f32 *)0x70003010)[3] = 1.0f;
        VAPPLY(sp, mat, sp);
        VSUB(sp, pos, sp);
        d = VuVecDot((f32 *)0x70003010, (f32 *)0x70003010);
        if (d <= (r + p->rad) * (r + p->rad)) {
            ip = p->idx;
            verts = p->verts;
            mapPolyMeshNearCount++;
            for (j = 0; j < p->count; j++) {
                copyq(tri[0], verts + *ip);
                ip++;
                VSUB(tri[2], verts + *ip, tri[0]);
                ip++;
                VSUB(tri[1], verts + *ip, tri[0]);
                ip += 2;
                do { /* do-while(0) + break: continue or else-if lose two insns of layout */
                    if (!VuSphereTriBoundHit(v[0], lim, tri)) {
                        mapPolyRejectBound++;
                        break;
                    }
                    if (!VuTriPlaneNear(v[0], lim, tri)) {
                        mapPolyRejectPlane++;
                        break;
                    }
                    if (!VuPointInTriangle(v[0], lim, tri)) {
                        mapPolyRejectInside++;
                        break;
                    }
                    mapPolyAcceptCount++;
                    MapPolyAdd((u8 *)tri);
                } while (0);
            }
        }
    }
    return 1;
}
