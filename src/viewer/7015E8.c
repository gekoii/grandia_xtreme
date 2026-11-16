/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "viewer/n35.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }
void VuSinCosAngle(f32 *, f32 *, s32);

void VwrPitchYawToDir(VuVec3 *out, f32 pitch, f32 yaw) {
    f32 c1;
    f32 s1;
    f32 c0;
    f32 s0;
    f32 a;
    f32 b;

    if (pitch >= 1.5690508f) {
        out->x = 0.0f;
        out->y = 1.0f;
        out->z = 0.0f;
    } else if (pitch <= -1.5690508f) {
        out->x = 0.0f;
        out->y = -1.0f;
        out->z = 0.0f;
    } else {
        VuSinCosAngle(&s0, &c0, (s32)(pitch * 10430.378f));
        VuSinCosAngle(&s1, &c1, (s32)(yaw * 10430.378f));
        a = s1 * c0;
        b = c1 * c0;
        out->x = -a;
        out->z = -b;
        a = fsqrt(a * a + b * b);
        out->y = -(-(s0 / c0) * a);
    }
}
