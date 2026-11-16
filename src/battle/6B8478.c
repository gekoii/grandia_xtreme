#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void EnvStageZoneInit(GphPart *src, EnvStageZone *z) {
    f32 v[3];
    f32 d;
    s32 a;

    v[0] = src->pos[0] + src->cullSphere[0];
    v[1] = src->pos[1] + src->cullSphere[1];
    v[2] = src->pos[2] + src->cullSphere[2];
    {
        f32 q = v[0] * v[0] + v[2] * v[2];
        d = fsqrt(q);
    }
    if (d < 1.0f) {
        z->angle = 0;
    } else {
        z->angle = BtlAtan2Angle(v[0], v[2]);
    }
    a = BtlAtan2Angle(src->cullSphere[3], d);
    z->angle = (z->angle - a - 0x800) & 0xFFFF;
    z->width = (a * 2 + 0x1000) & 0xFFFF;
    z->pitch = 0x4000 - BtlAtan2Angle(v[1] + 300.0f, d);
}
