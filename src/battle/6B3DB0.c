/* cflags: -ffast-math */
/* -ffast-math: sqrtf is a bare sqrt.s (no NaN check and sqrtf fallback call) */
#include "common.h"
#include "protos/battle.h"

typedef struct {
    u8 pad0[0x20];
    s32 pitch;
    s32 yaw;
} Rot;

void BtlChrDirToAngles(Rot *r, f32 x, f32 y, f32 z) {
    r->pitch = BtlAtan2Angle(-sqrtf(x * x + z * z), y);
    r->yaw = 0x8000 - BtlAtan2Angle(x, z);
}
