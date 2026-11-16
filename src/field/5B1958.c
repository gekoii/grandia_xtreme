#include "common.h"
#include "protos/field.h"
#include "field/n12.h"

void VuVecCopy(FldDbgVtx *, FldDbgVtx *);

void FldDebugDrawBox(f32 *pos, f32 *size, s32 color) {
    FldDbgVtx c[8];
    FldDbgVtx l[8];
    f32 hx, hy, hz;
    f32 x0, y0, z0, x1, y1, z1;
    s32 i;

    hx = size[0] * 0.5f;
    hy = size[1] * 0.5f;
    hz = size[2] * 0.5f;
    x0 = pos[0] - hx;
    y0 = pos[1] - hy;
    z0 = pos[2] - hz;
    x1 = pos[0] + hx;
    y1 = pos[1] + hy;
    z1 = pos[2] + hz;
    c[0].x = x0; c[0].y = y0; c[0].z = z0;
    c[1].x = x0; c[1].y = y1; c[1].z = z0;
    c[2].x = x1; c[2].y = y0; c[2].z = z0;
    c[3].x = x1; c[3].y = y1; c[3].z = z0;
    c[4].x = x0; c[4].y = y0; c[4].z = z1;
    c[5].x = x0; c[5].y = y1; c[5].z = z1;
    c[6].x = x1; c[6].y = y0; c[6].z = z1;
    c[7].x = x1; c[7].y = y1; c[7].z = z1;
    for (i = 0; i < 8; i++) {
        l[i].w = 1.0f;
    }
    VuVecCopy(&l[0], &c[1]);
    VuVecCopy(&l[1], &c[3]);
    VuVecCopy(&l[2], &c[7]);
    VuVecCopy(&l[3], &c[5]);
    VuVecCopy(&l[4], &c[1]);
    GphLine3DDraw(5, l, color, 1);
    VuVecCopy(&l[0], &c[0]);
    VuVecCopy(&l[1], &c[2]);
    VuVecCopy(&l[2], &c[6]);
    VuVecCopy(&l[3], &c[4]);
    VuVecCopy(&l[4], &c[0]);
    GphLine3DDraw(5, l, color, 1);
    VuVecCopy(&l[0], &c[0]);
    VuVecCopy(&l[1], &c[1]);
    GphLine3DDraw(2, l, color, 1);
    VuVecCopy(&l[0], &c[2]);
    VuVecCopy(&l[1], &c[3]);
    GphLine3DDraw(2, l, color, 1);
    VuVecCopy(&l[0], &c[4]);
    VuVecCopy(&l[1], &c[5]);
    GphLine3DDraw(2, l, color, 1);
    VuVecCopy(&l[0], &c[6]);
    VuVecCopy(&l[1], &c[7]);
    GphLine3DDraw(2, l, color, 1);
}
