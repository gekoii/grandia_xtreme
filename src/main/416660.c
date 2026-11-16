/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"

f32 PtySoftCap(f32 x, f32 w) {
    f32 a;
    f32 s;
    f32 h;

    a = x;
    a = (a < 0.0f) ? -a : a;
    s = -1.0f;
    if (x >= 0.0f) {
        s = 1.0f;
    }
    h = w * 0.5f;
    if (a <= h) {
        return x;
    }
    if (w * 1.5f <= a) {
        return w * s;
    }
    return (h + (a - h) * 0.5f) * s;
}
