/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"

void VuSinCosAngle(f32 *, f32 *, s32);

f32 PtclCurveEval(s32 arg0, f32 f) {
    u8 kind = arg0;
    f32 v[4];

    switch (kind) {
    case 0:
        f = 0.0f;
        break;
    case 1:
        break;
    case 2:
    case 3:
        VuSinCosAngle(&v[0], &v[1], (s32)(f * 16384.0f));
        f = v[0];
        if (kind == 2) {
            f = 1.0f - v[1];
        }
        break;
    case 4:
    case 5:
        VuSinCosAngle(&v[2], &v[3], (s32)(f * 32768.0f));
        if (kind == 4) {
            f = (1.0f - v[3]) * 0.5f;
        } else if (f < 0.5f) {
            f = v[2] * 0.5f;
        } else {
            f = (1.0f - v[2]) * 0.5f + 0.5f;
        }
        break;
    }
    return f;
}
