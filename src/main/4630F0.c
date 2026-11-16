/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"

s32 SndFloatToInt(f32);

u8 SndCalcAngle(f32 x, f32 y) {
    f32 a;

    if (y == 0.0f) {
        y = 0.0001f;
    }
    a = atanf(-x / y);
    if (y < 0.0f) {
        a += 3.1416f;
    }
    return SndFloatToInt(a * 128.0f / 3.1415927f);
}
