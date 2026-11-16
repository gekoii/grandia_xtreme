#include "common.h"
#include "battle/n33.h"
#include "protos/battle.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlVecGetPitchYaw(s32 *pitch, s32 *yaw, VuVec3 *v, s8 flip) {
    *pitch = BtlAtan2Angle(-fsqrt(v->x * v->x + v->z * v->z), v->y);
    if (flip) {
        *yaw = 0x8000 - BtlAtan2Angle(v->x, v->z);
    } else {
        *yaw = BtlAtan2Angle(v->x, v->z);
    }
}
