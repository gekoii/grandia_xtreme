#include "common.h"

typedef struct {
    u8 pad[8];
    f32 f8;
    f32 fc;
} Cam;

void VuSinCosAngle(f32 *, f32 *, s32);

void VuViewRollSet(s32 arg0) {
    f32 v[2];
    f32 b;
    f32 a;
    f32 *m;
    Cam *c;
    f32 x;
    f32 y;

    c = *(Cam * volatile *)0x700000EC;
    b = c->f8;
    a = c->fc;
    VuSinCosAngle(&v[0], &v[1], arg0);
    y = v[1];
    x = v[0];
    m = (f32 *)0x70000300;
    m[1] = x * a;
    m[0] = y * b;
    m[5] = y * a;
    m[4] = -x * b;
}
