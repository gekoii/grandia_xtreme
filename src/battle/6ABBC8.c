/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"
#include "protos/battle.h"

void PtclBtlSprSetSpeed(PtclBtlSpr *arg0, f32 arg1, f32 arg2) {
    f32 s, inv, a, b, c;

    s = arg1 + (rand() >> 15) * (1.0f / 65536.0f) * arg2;
    b = arg0->pos[0] * s;
    c = arg0->pos[1] * s;
    a = arg0->pos[2] * s;
    inv = 1.0f / (arg0->life - 1);
    arg0->vel[2] = a;
    arg0->vel[0] = b;
    arg0->vel[1] = c;
    arg0->accel[2] = -a * inv;
    arg0->accel[0] = -b * inv;
    arg0->accel[1] = -c * inv;
}
