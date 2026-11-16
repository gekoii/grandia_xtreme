/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n07.h"

void GphWipeShutter(GphWipe *w) {
    f32 n = w->frame;
    f32 h = n * 20.0f;
    f32 base;
    f32 sgn;
    f32 y;
    f32 y2;
    f32 d;
    GphBatchQuad *r;
    s32 mode = w->mode;

    if (h >= 224.0f) {
        h = 224.0f;
        w->state = 2;
    }
    base = 224.0f;
    if (mode & 1) {
        sgn = -1.0f;
        y = 0.0f;
    } else {
        sgn = 1.0f;
        y = -224.0f;
        base = 448.0f;
    }
    if (n == 0.0f) {
        y2 = base;
    } else {
        d = h * sgn;
        y2 = base - d;
        y += d;
    }
    r = gphWipeSprites;
    r[0].type = 1;
    r[0].v[0].y = y;
    r[0].v[0].r = 0;
    r[0].v[0].g = 0;
    r[0].v[1].y = y + 224.0f;
    r[0].v[0].b = 0;
    r[0].v[0].a = 0x80;
    r[0].v[0].x = 0;
    r[0].v[1].x = 0x280;
    r[0].z = 0;
    r++;
    r[0].type = 1;
    r[0].v[0].a = 0x80;
    r[0].v[0].y = y2;
    r[0].v[1].x = 0x280;
    r[0].v[1].y = y2 + 224.0f;
    r[0].v[0].r = 0;
    r[0].v[0].g = 0;
    r[0].v[0].b = 0;
    r[0].v[0].x = 0;
    r[0].z = 0;
    r[1].type = 0;
}
