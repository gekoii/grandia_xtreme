/* Port-only C for EvtAreaHitBox (0x005B2BC0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"

extern FVECTOR evtAreaBoxNormals[6];
extern FVECTOR evtAreaBoxHalf;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
s32 EvtAreaHitBox(f32 *p, f32 r, EvtArea *e);

/* m * v, all four components (vmulax/vmadday/vmaddaz/vmaddw) */
static void mulMtxVec(FVECTOR *m, f32 *dst, f32 *v) {
    FVECTOR t;
    s32 i;

    for (i = 0; i < 4; i++) {
        t[i] = m[0][i] * v[0] + m[1][i] * v[1] + m[2][i] * v[2] + m[3][i] * v[3];
    }
    __builtin_memcpy(dst, t, 16);
}

/* Whether a sphere at p of radius r is inside all six faces of the box,
   that is no face is more than r in front of it. With a world matrix
   (flags & 4) the faces go through it; else the box sits at pos (plus the
   linked model's position), unrotated. */
s32 EvtAreaHitBox(f32 *p, f32 r, EvtArea *e) {
    FVECTOR half;
    FVECTOR center;
    FVECTOR n;
    FVECTOR v;
    f32 *box = e->shapeData.box;
    s32 i;

    /* half extents; w stays box[3], and the matrix path uses it */
    half[0] = box[0] * evtAreaBoxHalf[0];
    half[1] = box[1] * evtAreaBoxHalf[1];
    half[2] = box[2] * evtAreaBoxHalf[2];
    half[3] = box[3];
    half[0] = half[0] * e->scale[0];
    half[1] = half[1] * e->scale[1];
    half[2] = half[2] * e->scale[2];
    if (e->flags & 4) {
        for (i = 0; i < 6; i++) {
            v[0] = half[0] * evtAreaBoxNormals[i][0] + e->ofs[0];
            v[1] = half[1] * evtAreaBoxNormals[i][1] + e->ofs[1];
            v[2] = half[2] * evtAreaBoxNormals[i][2] + e->ofs[2];
            v[3] = half[3];
            mulMtxVec(e->mtx, n, evtAreaBoxNormals[i]);
            mulMtxVec(e->mtx, v, v);
            v[0] = p[0] - v[0];
            v[1] = p[1] - v[1];
            v[2] = p[2] - v[2];
            if (r < VuVecDot(n, v)) {
                return 0;
            }
        }
        return 1;
    }
    if (e->link != NULL) {
        center[0] = e->pos[0] + e->link->pos[0];
        center[1] = e->pos[1] + e->link->pos[1];
        center[2] = e->pos[2] + e->link->pos[2];
        center[3] = e->pos[3];
    } else {
        VuVecCopy(center, e->pos);
    }
    for (i = 0; i < 6; i++) {
        v[0] = half[0] * evtAreaBoxNormals[i][0] + center[0];
        v[1] = half[1] * evtAreaBoxNormals[i][1] + center[1];
        v[2] = half[2] * evtAreaBoxNormals[i][2] + center[2];
        v[3] = half[3];
        v[0] = p[0] - v[0];
        v[1] = p[1] - v[1];
        v[2] = p[2] - v[2];
        if (r < VuVecDot(evtAreaBoxNormals[i], v)) {
            return 0;
        }
    }
    return 1;
}
