/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"


f32 CamApproachSpeedRel(f32 dist, f32 cur, f32 target, f32 accel, f32 maxv) {
    f32 t;
    f32 r;
    f32 m;
    f32 v;

    v = target - cur;
    /* goto into the do-while, do-while(0) wraps and empty barriers: the original block layout and scheduling (the if/else form of CamApproachSpeed is 61 words off) */
    t = 0.0f;
    do { if (dist < accel * 0.1f) { v = t; goto end; } } while (0);
    goto test;
    do {
        t = v * 0.5f * v / accel;
        if (v > 0.0f) {
            r = t / dist;
        } else {
            r = 1.0f / (dist + t);
        }
        do {} while (0);
        if (!(r > 1.0f)) {
            if (r < 1.0f) {
                m = 1.0 / r < 1.0f ? 1.0 / r : 1.0f;
                v += accel * m;
            }
        } else {
            do { m = r > 1.0f ? r : 1.0f; } while (0);
            v -= accel * m;
        }
        goto end;
test:
        if (dist < accel) { v = dist; goto end; }
    } while (!(dist < accel + accel));
    v = accel;
end:
    v += cur;
    if (maxv < v) {
        v = maxv;
    }
    if (dist < v) {
        v = dist;
    }
    return fabsf(v);
}
