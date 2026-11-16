/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define CLAMP1(c) ((c) < -1.0f ? -1.0f : ((c) > 1.0f ? 1.0f : (c)))
void BtlVecGetPitchYawAcos(f32 *v, s32 *pitch, s32 *yaw) {

    if (yaw != NULL) {
        f32 a, c, d, r;
        d = fsqrt(v[0] * v[0] + v[2] * v[2]);
        if (d <= 0.001f) {
            *yaw = 0;
        } else {
            c = v[2] / d;
            r = CLAMP1(c);
            if (v[0] > 0.0f) {
                a = -acosf(r);
            } else {
                a = acosf(r);
            }
            *yaw = (s32)(a * 10430.381f) & 0xFFFF;
        }
    }
    if (pitch != NULL) {
        f32 c, d, r;
        d = fsqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (d <= 0.001f) {
            *pitch = 0;
        } else {
            c = v[1] / d;
            r = CLAMP1(c);
            *pitch = ((s32)(-asinf(r) * 10430.381f) + 0x4000) & 0xFFFF;
        }
    }
}
