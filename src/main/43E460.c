/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"

void VuMatrixToEuler(s32 *out, f32 *m) {
    f32 m8;
    f32 m0;
    f32 m4;
    f32 a;
    f32 b;
    f32 c;

    m8 = m[2];
    m0 = m[0];
    m4 = m[1];
    a = atan2f(m[6], m[10]);
    b = asinf(m8);
    if (__builtin_fabsf(m8) > 0.99999f) {
        a = atan2f(-m[9], m[5]);
        c = 0.0f;
    } else {
        c = asinf(m4 / cos(b));
        if (m0 < 0.0f) {
            if (c > 0.0f) {
                c = 3.1415927f - c;
            } else {
                c = -3.1415927f - c;
            }
        }
    }
    out[0] = a * 10430.378f;
    out[1] = b * 10430.378f;
    out[2] = c * 10430.378f;
}
