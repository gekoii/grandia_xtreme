/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "sys.h"


void PtclBtlSprDrawKeyed(PtclBtlSpr *e, s32 n, PtclSparkKeys *k, u8 (*c)[4]) {
    s32 t;
    s32 a;
    s32 b;

    for (; n > 0; n--, e++) {
        b = k->life + e->lifeExtra;
        a = e->life - 1;
        t = b - a;
        if (e->life > 0) {
            if (t < k->t1) {
                f32 f = 1.0f - (f32)t / (f32)k->t1;
                sprState.sx = (k->scale[0] - k->scale[1]) * f + k->scale[1];
                sprState.col[0].r = (c[0][0] - c[1][0]) * f + c[1][0];
                sprState.col[0].g = (c[0][1] - c[1][1]) * f + c[1][1];
                sprState.col[0].b = (c[0][2] - c[1][2]) * f + c[1][2];
                sprState.col[0].a = (c[0][3] - c[1][3]) * f + c[1][3];
            } else if (t < k->t2) {
                f32 f = 1.0f - (f32)(t - k->t1) / (f32)(k->t2 - k->t1);
                sprState.sx = (k->scale[1] - k->scale[2]) * f + k->scale[2];
                sprState.col[0].r = (c[1][0] - c[2][0]) * f + c[2][0];
                sprState.col[0].g = (c[1][1] - c[2][1]) * f + c[2][1];
                sprState.col[0].b = (c[1][2] - c[2][2]) * f + c[2][2];
                sprState.col[0].a = (c[1][3] - c[2][3]) * f + c[2][3];
            } else {
                f32 f = 1.0f - (f32)(t - k->t2) / (f32)(k->life - k->t2 + e->lifeExtra);
                sprState.sx = (k->scale[2] - k->scale[3]) * f + k->scale[3];
                sprState.col[0].r = (c[2][0] - c[3][0]) * f + c[3][0];
                sprState.col[0].g = (c[2][1] - c[3][1]) * f + c[3][1];
                sprState.col[0].b = (c[2][2] - c[3][2]) * f + c[3][2];
                sprState.col[0].a = (c[2][3] - c[3][3]) * f + c[3][3];
            }
            sprState.sy = sprState.sx = sprState.sx * e->scale;
            if (e->flags & 1) {
                sprState.sx = -sprState.sx;
            }
            if (e->flags & 2) {
                sprState.sy = -sprState.sy;
            }
            SprDrawRect3D(&ptclGlowRect, -1, -1, e->pos);
        }
    }
}
