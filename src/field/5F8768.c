/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"


static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 FldDirXZToAngle(f32 x, f32 y) {
    f32 r;
    f32 a;

    r = 1.0f / fsqrt(x * x + y * y);
    y *= r;
    x *= r;
    if (y > 1.0f) {
        y = 1.0f;
    }
    if (y < -1.0f) {
        y = -1.0f;
    }
    if (x > 0.0f) {
        a = -asinf(-y);
    } else {
        a = asinf(-y);
    }
    return a * 57.295776f * 182.04445f;
}
