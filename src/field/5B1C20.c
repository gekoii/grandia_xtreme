#include "common.h"
#include "protos/field.h"
#include "field/n12.h"

void VuSinCosDeg(f32 *, f32 *, f32);

void FldDebugDrawCylinder(f32 *pos, s32 color, f32 r, f32 h) {
    FldDbgVtx v[9];
    f32 s0, c0;
    f32 s1, c1;
    f32 s2, c2;
    s32 i;
    s32 j;
    FldDbgVtx *p;

    for (i = 0, p = v; i < 9; i++, p++) {
        VuSinCosDeg(&s0, &c0, i * 45.0f);
        {
            f32 x = pos[0];
            f32 y = pos[1];
            f32 z = pos[2];
            p->x = x + c0 * r;
            p->y = y;
            p->z = z + s0 * r;
            p->w = 1.0f;
        }
    }
    GphLine3DDraw(9, v, color, 1);
    for (i = 0, p = v; i < 9; i++, p++) {
        VuSinCosDeg(&s1, &c1, i * 45.0f);
        {
            f32 x = pos[0];
            f32 y = pos[1];
            f32 z = pos[2];
            p->x = x + c1 * r;
            p->y = y + h;
            p->z = z + s1 * r;
            p->w = 1.0f;
        }
    }
    GphLine3DDraw(9, v, color, 1);
    for (j = 0; j < 9; j++) {
        VuSinCosDeg(&s2, &c2, j * 45.0f);
        v[0].x = pos[0] + c2 * r;
        v[0].y = pos[1];
        v[0].z = pos[2] + s2 * r;
        v[0].w = 1.0f;
        v[1].x = v[0].x;
        v[1].y = v[0].y + h;
        v[1].z = v[0].z;
        v[1].w = 1.0f;
        GphLine3DDraw(2, v, color, 1);
    }
}
