#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "gph.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void DbgBtlDrawCylinder(f32 x, f32 y, f32 z, f32 r, f32 h, u32 c) {
    GphQuad3D buf[17];
    f32 cb;
    f32 sb;
    f32 ca;
    f32 sa;
    u8 *cp;
    GphQuad3D *p;
    s32 i;

    cp = (u8 *)&c;
    buf[0].type = 0x11;
    buf[16].type = 0;
    buf[0].col[0].r = cp[3];
    buf[0].col[0].g = cp[2];
    buf[0].col[0].b = cp[1];
    buf[0].col[0].a = (u8)c;
    buf[0].col[1].r = cp[3];
    buf[0].col[1].g = cp[2];
    buf[0].col[1].b = cp[1];
    buf[0].col[1].a = (u8)c;
    buf[0].col[2].r = cp[3];
    buf[0].col[2].g = cp[2];
    buf[0].col[2].b = cp[1];
    buf[0].col[2].a = (u8)c;
    buf[0].col[3].r = cp[3];
    buf[0].col[3].g = cp[2];
    buf[0].col[3].b = cp[1];
    buf[0].col[3].a = (u8)c;
    buf[0].pos[2].y = y + h;
    buf[0].pos[0].y = y + h;
    buf[0].pos[3].y = y;
    buf[0].pos[1].y = y;
    VuSinCosAngle(&sa, &ca, -0x1000);
    buf[0].pos[0].x = buf[0].pos[1].x = x + sa * r;
    buf[0].pos[0].z = buf[0].pos[1].z = z + ca * r;
    VuSinCosAngle(&sa, &ca, 0);
    buf[0].pos[2].x = buf[0].pos[3].x = x + sa * r;
    buf[0].pos[2].z = buf[0].pos[3].z = z + ca * r;
    p = &buf[1];
    for (i = 1; i < 16; i++) {
        p->type = 0x11;
        p->col[0].r = ((u8 *)&c)[3];
        p->col[0].g = ((u8 *)&c)[2];
        p->col[0].b = ((u8 *)&c)[1];
        p->col[0].a = (u8)c;
        p->col[1].r = ((u8 *)&c)[3];
        p->col[1].g = ((u8 *)&c)[2];
        p->col[1].b = ((u8 *)&c)[1];
        p->col[1].a = (u8)c;
        p->col[2].r = ((u8 *)&c)[3];
        p->col[2].g = ((u8 *)&c)[2];
        p->col[2].b = ((u8 *)&c)[1];
        p->col[2].a = (u8)c;
        p->col[3].r = ((u8 *)&c)[3];
        p->col[3].g = ((u8 *)&c)[2];
        p->col[3].b = ((u8 *)&c)[1];
        p->col[3].a = (u8)c;
        p->pos[2].y = y + h;
        p->pos[0].y = y + h;
        p->pos[3].y = y;
        p->pos[1].y = y;
        p->pos[0].x = p->pos[1].x = (p - 1)->pos[2].x;
        p->pos[0].z = p->pos[1].z = (p - 1)->pos[2].z;
        VuSinCosAngle(&sb, &cb, i << 12);
        p->pos[2].x = p->pos[3].x = x + sb * r;
        p->pos[2].z = p->pos[3].z = z + cb * r;
        p++;
    }
    GphBatchDrawQuads3D(buf);
}
