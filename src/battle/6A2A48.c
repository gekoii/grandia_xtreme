/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "sys.h"


void FxRegenDraw(void) {
    BtlUnit *u = btlUnits;
    FxRegen *p = fxRegen;
    SprState *ctx;
    FxRegenPart *e;
    VuVec4 v;
    s32 i;
    s32 j;
    s8 t;
    f32 a;
    f32 g;
    f32 h;
    f32 zero;

    a = 0.0f;
    GphTexBind(0x41);
    SprBegin(a, a);
    zero = a;
    sprState.blend = 2;
    for (i = 0; i < 16; i++, u++, p++) {
        ctx = &sprState;
        if (u->state <= 0) {
            continue;
        }
        if (p->parts == NULL) {
            continue;
        }
        if (u->alpha <= zero) {
            continue;
        }
        e = p->parts;
        ctx->col[0].a = u->alpha * 128.0f;
        t = 30 - p->timer;
        if (t < 5) {
            a = t * 0.6f;
        } else if (t < 15) {
            a = 3.0f - (t - 5) * 0.2f;
        } else {
            a = 1.0f - (t - 15) * (1.0f / 15.0f);
        }
        if (t < 5) {
            g = t * 0.2f;
            ctx->col[0].r = g * 128.0f + zero;
            ctx->col[0].g = g * 64.0f + 64.0f;
        } else if (t < 10) {
            h = (t - 5) * 0.2f;
            ctx->col[0].g = h * -64.0f + 128.0f;
            ctx->col[0].r = h * -128.0f + 128.0f;
        } else {
            ctx->col[0].r = 0;
            g = (t - 10) * 0.05f;
            ctx->col[0].g = g * -64.0f + 64.0f;
        }
        ctx->col[0].b = 0x80;
        for (j = 0; j < 31; j++, e++) {
            sprState.angle = e->angle;
            v.x = e->pos[0];
            v.y = e->pos[1];
            v.z = e->pos[2];
            sprState.sx = sprState.sy = a * e->scale;
            SprDrawRect3D(&ptclGlowRect, -1, -1, &v);
        }
    }
    SprFlush();
}
