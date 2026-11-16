/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"


s32 PtyRollGrowth(s32 a0, s32 a1, s32 a2, s32 a3) {
    f32 sum;
    f32 t;
    f32 r;

    r = (rand() >> 15) * (1.0f / 65536.0f);
    sum = a2 + a3;
    t = (a1 - sum * 0.5f * a0) / sum;
    if (t > 0.0f) {
        if (t > 1.0f) {
            t = 1.0f;
        }
        r = r - r * ((rand() >> 15) * (1.0f / 65536.0f)) * t;
    } else if (t < 0.0f) {
        if (t < -1.0f) {
            t = -1.0f;
        }
        r = r + (1.0f - r) * ((rand() >> 15) * (1.0f / 65536.0f)) * -t;
    }
    return (a2 - a3) * r + 0.5f + a3;
}
