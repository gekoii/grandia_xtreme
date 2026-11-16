/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n28.h"

void BtlFadeUpdate(void) {
    BtlFade *f = &btlFade;
    f32 r;

    if (f->dur > 0) {
        f->t++;
        if (f->t >= f->dur) {
            f->dur = 0;
            f->t = 0;
            f->quad.col[3].w = f->to.w;
            f->quad.col[1].w = f->to.w;
            f->quad.col[2].w = f->to.w;
            f->quad.col[0].w = f->to.w;
            f->from.w = f->to.w;
            return;
        }
        r = (f32)f->t / (f32)f->dur;
        f->quad.col[0].c.r = (f->to.c.r - f->from.c.r) * r + f->from.c.r;
        f->quad.col[0].c.a = (f->to.c.a - f->from.c.a) * r + f->from.c.a;
        f->quad.col[0].c.b = (f->to.c.b - f->from.c.b) * r + f->from.c.b;
        f->quad.col[0].c.g = (f->to.c.g - f->from.c.g) * r + f->from.c.g;
        f->quad.col[3].w = f->quad.col[0].w;
        f->quad.col[1].w = f->quad.col[0].w;
        f->quad.col[2].w = f->quad.col[0].w;
    }
}
