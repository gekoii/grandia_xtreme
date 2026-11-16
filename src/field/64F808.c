/* as: ee-gcc2.9-991111-01 */
#include "common.h"

f32 MiniTriangleWave(f32 x) {
    if (x > 0.0f) {
        while (x > 1.0f) {
            x -= 1.0f;
        }
    } else {
        while (x < 0.0f) {
            x += 1.0f;
        }
    }
    x = x + x;
    if (x < 1.0f) {
        return x;
    }
    return 2.0f - x;
}
