/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"

f32 CamApproachSpeed(f32 dist, f32 speed, f32 accel, f32 maxv) {
    f32 t;
    f32 r;
    f32 m;

    /* goto into the do-while, do-while(0) wraps and empty barriers: the original block layout and scheduling (the if/else form of CamApproachSpeed is 61 words off) */
    t = 0.0f;
    do { if (dist < accel * 0.1f) { speed = t; goto end; } } while (0);
    goto test;
    do {
        t = speed * 0.5f * speed / accel;
        r = t / dist;
        do {} while (0);
        if (r > 1.0f) {
            do { m = 1.0f; do {} while (0); if (r > 1.0f) m = r; } while (0);
            speed -= accel * m;
        } else if (r < 1.0f) {
            speed += accel;
        }
        goto end;
test:
        if (dist < accel) { speed = dist; goto end; }
    } while (!(dist < accel + accel));
    speed = accel;
end:
    if (maxv < speed) {
        speed = maxv;
    }
    if (dist < speed) {
        speed = dist;
    }
    if (speed < 0.0f) {
        speed = 0.0f;
    }
    return speed;
}
