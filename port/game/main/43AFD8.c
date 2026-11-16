/* Port-only C for GphProjTexCalcMtx (0x0043AFD8), which the decompilation still has in assembly. */
#include "common.h"
#include "main/n05.h"
#include "vu0_port.h"

void VuVecNormalize(f32 *dst, f32 *src);
void VuMtxMul(f32 *dst, f32 *a, f32 *b);

void GphProjTexCalcMtx(GphProjTexParam *p);

/*
 * The projector's view matrix: a yaw then a pitch that turn -dir onto the
 * z axis, after a translation by -pos; x and y negated. mtx[0] gets it,
 * mtx[1] the same with x, y, z scaled by 1 / radius.
 */
void GphProjTexCalcMtx(GphProjTexParam *p) {
    FMATRIX m;
    FMATRIX r;
    FVECTOR dir;
    FVECTOR v;
    FVECTOR h;
    f32 c;
    f32 s;
    s32 i;

    VuVecNormalize(dir, p->dir);
    dir[0] = dir[0] * -1.0f;
    dir[1] = dir[1] * -1.0f;
    dir[2] = dir[2] * -1.0f;
    if (dir[0] == 0.0f && dir[2] == 0.0f) {
        c = 1.0f;
        s = 0.0f;
    } else {
        VuCopy4(v, dir);
        v[1] = 0.0f;
        VuVecNormalize(h, v);
        c = h[2];
        s = h[0];
    }
    /* Yaw */
    VuUnitMtx(&m[0][0]);
    m[2][2] = c;
    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    VuApplyMtx(v, dir, &m[0][0]);
    if (v[1] != 0.0f) {
        /* Pitch */
        v[0] = 0.0f;
        VuVecNormalize(h, v);
        VuUnitMtx(&r[0][0]);
        r[1][2] = h[1];
        r[2][2] = h[2];
        r[1][1] = h[2];
        r[2][1] = -h[1];
        VuMtxMul(&m[0][0], &r[0][0], &m[0][0]);
    }
    /* Translation by -pos (the vector's w is replaced by 1) */
    VuCopy4(v, p->pos);
    v[0] = v[0] * -1.0f;
    v[1] = v[1] * -1.0f;
    v[2] = v[2] * -1.0f;
    v[3] = 1.0f;
    VuUnitMtx(&r[0][0]);
    r[3][0] += v[0];
    r[3][1] += v[1];
    r[3][2] += v[2];
    VuMtxMul(&m[0][0], &m[0][0], &r[0][0]);
    for (i = 0; i < 4; i++) {
        m[i][0] = -m[i][0];
        m[i][1] = -m[i][1];
    }
    VuCopyMtx(p->mtx[0], m);
    VuCopyMtx(p->mtx[1], m);
    v[0] = 1.0f / p->radius[0];
    v[1] = 1.0f / p->radius[1];
    v[2] = 1.0f / p->radius[2];
    for (i = 0; i < 4; i++) {
        p->mtx[1][i][0] *= v[0];
        p->mtx[1][i][1] *= v[1];
        p->mtx[1][i][2] *= v[2];
    }
}
