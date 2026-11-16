#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    u8 pad[8];
} V;

typedef struct {
    u8 pad0[0x18];
    V v[4];
} Obj;

void VuSinCosDeg(f32 *, f32 *, f32);

void GsQuadRotate(Obj *o, s32 x, s32 y, s32 w, s32 h, f32 ang) {
    f32 sc[2];
    f32 l;
    f32 t;
    f32 r;
    f32 b;

    w >>= 1;
    h >>= 1;
    l = -w;
    t = -h;
    b = h;
    r = w;
    VuSinCosDeg(&sc[0], &sc[1], ang);
    o->v[0].x = x + (s32)(l * sc[1] - t * sc[0]);
    o->v[0].y = y + (s32)(l * sc[0] + t * sc[1]);
    o->v[1].x = x + (s32)(l * sc[1] - b * sc[0]);
    o->v[1].y = y + (s32)(l * sc[0] + b * sc[1]);
    o->v[2].x = x + (s32)(r * sc[1] - t * sc[0]);
    o->v[2].y = y + (s32)(r * sc[0] + t * sc[1]);
    o->v[3].x = x + (s32)(r * sc[1] - b * sc[0]);
    o->v[3].y = y + (s32)(r * sc[0] + b * sc[1]);
}
