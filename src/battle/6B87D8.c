#include "common.h"
#include "battle/camera.h"
#include "battle/env.h"
#include "protos/battle.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 EnvStageZoneIsVisible(EnvStageZone *z) {
    BtlCamParam *c = &btlCamParam;
    f32 d;
    f32 y;

    if (z->width < ((BtlAtan2Angle(c->eye[0], c->eye[2]) - z->angle) & 0xFFFF)) {
        return 1;
    }
    y = c->eye[0] * c->eye[0] + c->eye[2] * c->eye[2];
    d = fsqrt(y);
    if (BtlAtan2Angle(d, c->eye[1]) < z->pitch || ((BtlAtan2Angle(c->eye[0] - c->target[0], c->eye[2] - c->target[2]) - (z->angle + z->width / 2) + 0x2800) & 0xFFFF) > 0x5000) {
        return 1;
    }
    return 0;
}
