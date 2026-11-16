#include "common.h"
#include "protos/battle.h"

void VuSinCosAngle(f32 *, f32 *, s32);
#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

void BtlXZAddRandomOffset(f32 *x, f32 *z, f32 base, f32 range) {
    f32 s;
    f32 c;
    f32 t;

    VuSinCosAngle(&s, &c, rand() >> 15);
    t = 1.0f - RND01();
    t = 1.0f - t * t;
    t = base + range * t;
    *x += s * t;
    *z += c * t;
}
