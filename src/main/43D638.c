#include "common.h"
#include "main/n06.h"
#include "protos/main.h"

void GphPoly3DDraw(s32, VuVec4 *, s32, s32);

void GphBoxDraw(VuVec3 *c, VuVec3 *s, s32 color, s32 lines) {
    VuVec4 v[8];
    VuVec4 p[8];
    f32 hx;
    f32 hy;
    f32 hz;
    f32 x0, y0, z0, x1, y1, z1;

    hx = s->x * 0.5f;
    hy = s->y * 0.5f;
    hz = s->z * 0.5f;
    x0 = c->x - hx;
    y0 = c->y - hy;
    z0 = c->z - hz;
    x1 = c->x + hx;
    y1 = c->y + hy;
    z1 = c->z + hz;
    v[0].x = x0; v[0].y = y0; v[0].z = z0; v[0].w = 1.0f;
    v[1].x = x0; v[1].y = y1; v[1].z = z0; v[1].w = 1.0f;
    v[2].x = x1; v[2].y = y0; v[2].z = z0; v[2].w = 1.0f;
    v[3].x = x1; v[3].y = y1; v[3].z = z0; v[3].w = 1.0f;
    v[4].x = x0; v[4].y = y0; v[4].z = z1; v[4].w = 1.0f;
    v[5].x = x0; v[5].y = y1; v[5].z = z1; v[5].w = 1.0f;
    v[6].x = x1; v[6].y = y0; v[6].z = z1; v[6].w = 1.0f;
    v[7].x = x1; v[7].y = y1; v[7].z = z1; v[7].w = 1.0f;
    if (lines) {
        p[0] = v[0];
        p[1] = v[1];
        p[2] = v[3];
        p[3] = v[2];
        p[4] = v[0];
        GphLine3DDraw(5, p, color, 1);
        p[0] = v[4];
        p[1] = v[5];
        p[2] = v[7];
        p[3] = v[6];
        p[4] = v[4];
        GphLine3DDraw(5, p, color, 1);
        p[0] = v[0];
        p[1] = v[4];
        p[2] = v[1];
        p[3] = v[5];
        p[4] = v[3];
        p[5] = v[7];
        p[6] = v[2];
        p[7] = v[6];
        GphLine3DDraw(8, p, color, 1);
    } else {
        p[0] = v[0];
        p[1] = v[1];
        p[2] = v[2];
        p[3] = v[3];
        GphPoly3DDraw(4, p, color, 1);
        p[0] = v[4];
        p[1] = v[6];
        p[2] = v[5];
        p[3] = v[7];
        GphPoly3DDraw(4, p, color, 1);
        p[0] = v[0];
        p[1] = v[4];
        p[2] = v[1];
        p[3] = v[5];
        p[4] = v[3];
        p[5] = v[7];
        p[6] = v[2];
        p[7] = v[6];
        GphPoly3DDraw(8, p, color, 1);
    }
}
