/* as: ee-gcc2.9-991111-01 */
#include "common.h"

#define WRAP(x)                     \
    if ((x) < 0.0f) {               \
        (x) += 6.283184f;           \
    } else if ((x) >= 6.283184f) {  \
        (x) -= 6.283184f;           \
    }

f32 VwrAngleDiff(f32 a, f32 b) {
    f32 d1;
    f32 d2;
    f32 r;

    WRAP(a);
    WRAP(b);
    d1 = b - a;
    WRAP(d1);
    d2 = a - b;
    WRAP(d2);
    if (d1 == d2) {
        r = d1;
    } else if (d1 < d2) {
        r = d1;
    } else {
        r = -d2;
    }
    return r;
}
