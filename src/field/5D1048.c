/* as: ee-gcc2.9-991111-01 */
#include "common.h"

f32 CamAngleWrap180(f32 a) {
    if (a > 0.0f) {
        while (a > 180.0f) {
            a -= 360.0f;
        }
    } else {
        while (a < -180.0f) {
            a += 360.0f;
        }
    }
    return a;
}
