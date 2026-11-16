/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

f32 VuVecDot(f32 *, f32 *);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

f32 CamVecPitch(f32 *v) {
    f32 y = v[1];
    f32 d;
    v[1] = 0.0f;
    d = fsqrt(VuVecDot(v, v));
    return atanf(y / d);
}
