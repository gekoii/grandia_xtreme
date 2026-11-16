/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"

typedef struct {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    s32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
    f32 f40;
    f32 f44;
    f32 f48;
    f32 f4C;
    f32 f50;
    f32 f54;
    f32 f58;
    f32 f5C;
    f32 f60;
    f32 f64;
    f32 f68;
    f32 f6C;
    f32 f70;
} Out;

typedef struct {
    u8 pad[0x40];
    u64 f40;
} In1;

typedef struct {
    u8 pad[0x40];
    f32 f40;
    f32 f44;
    f32 f48;
} In2;

typedef struct {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
} In3;


void VuViewClipSetup(Out *o, In1 *a, In2 *b, In3 *c, f32 z) {
    f32 t;
    u64 k;

    o->f10 = 0;
    k = (a->f40 >> 24) & 0xF;
    switch (k) {
    case 0:
        o->f14 = 4294967296.0f;
        break;
    case 1:
        o->f14 = 16777215.0f;
        break;
    default:
        o->f14 = 65535.0f;
        break;
    }
    o->f0 = c->f8;
    o->f4 = c->fC;
    o->f8 = c->f18;
    o->fC = c->f1C;
    o->f1C = b->f40;
    o->f20 = b->f44;
    o->f24 = b->f48;
    o->f18 = c->f20;
    o->f28 = c->f24;
    o->f2C = c->f28;

    t = c->f10 / c->f18;
    o->f30 = sqrtf(t * t + c->f20 * c->f20) / c->f20;
    o->f40 = -t / c->f20;
    t = (c->f8 - c->f10) / c->f18;
    o->f34 = sqrtf(t * t + c->f20 * c->f20) / c->f20;
    o->f44 = t / c->f20;
    t = (c->fC - c->f14) / c->f1C;
    o->f38 = sqrtf(t * t + c->f20 * c->f20) / c->f20;
    o->f48 = -t / c->f20;
    t = c->f14 / c->f1C;
    o->f3C = sqrtf(t * t + c->f20 * c->f20) / c->f20;
    o->f4C = t / c->f20;
    o->f50 = o->f28 * (o->f14 - z);
    o->f54 = c->f18 * 16.0f;
    o->f58 = -c->f1C * 16.0f;
    o->f5C = (c->f24 * (c->f2C - z)) / c->f20;
    o->f60 = b->f40 * b->f40;
    o->f64 = b->f44 * o->f60 * 16.0f;
    o->f68 = (c->f0 + 2048.0f + c->f10) * 16.0f;
    o->f6C = (c->f4 + 2048.0f + c->f14) * 16.0f;
    o->f70 = c->f20;
}
