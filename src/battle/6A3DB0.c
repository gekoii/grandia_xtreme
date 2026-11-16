/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "sys.h"


void FxBindDraw(void) {
    BtlUnit *u = btlUnits;
    FxBind *p = fxBind;
    FxBindPart *e;
    PtclCellUv *r;
    VuVec4 v;
    SprRect rc;
    s32 i;
    s32 j;
    s8 t;
    f32 a;
    f32 k;
    f32 c64;

    GphTexBind(0x38);
    c64 = 64.0f;
    GphAnimSetSelect(ptclAnimSetTex38);
    SprBegin(0.0f, 0.0f);
    sprState.blend = 2;
    for (i = 0; i < 16; i++, u++, p++) {
        if (u->state <= 0) {
            continue;
        }
        if (p->parts == NULL) {
            continue;
        }
        if (u->alpha <= 0.0f) {
            continue;
        }
        e = p->parts;
        for (j = 0; j < 32; j++, e++) {
            if (e->life <= 0) {
                continue;
            }
            t = 16 - e->life;
            if (t < 4) {
                sprState.sx = t * 0.125f + 1.0f;
            } else if (t < 8) {
                sprState.sx = (t - 4) * -0.125f + 1.5f;
            } else {
                sprState.sx = 1.0f;
            }
            a = sprState.sx;
            sprState.sy = a * e->sy;
            sprState.sx = a * e->sx;
            if (t < 4) {
                k = t * 0.25f;
                sprState.col[0].r = k * 127.0f + 128.0f;
                sprState.col[0].g = k * 100.0f + 0.0f;
                sprState.col[0].b = k * 36.0f + c64;
                sprState.col[0].a = k * c64 + 128.0f;
            } else if (t < 8) {
                k = (t - 4) * 0.25f;
                sprState.col[0].r = 255;
                sprState.col[0].g = k * -40.0f + 100.0f;
                sprState.col[0].b = k * -45.0f + 100.0f;
                sprState.col[0].a = k * -64.0f + 192.0f;
            } else {
                k = (t - 8) * 0.125f;
                sprState.col[0].r = 255;
                sprState.col[0].g = k * -60.0f + 60.0f;
                sprState.col[0].b = k * -55.0f + 55.0f;
                sprState.col[0].a = k * -128.0f + 128.0f;
            }
            sprState.col[0].a = sprState.col[0].a * u->alpha;
            sprState.flags |= e->flip;
            v.x = e->pos[0];
            v.y = e->pos[1];
            v.z = e->pos[2];
            sprState.angle = e->angle;
            r = GphAnimGetCellUv(e->anim.cell);
            rc.w = 2;
            rc.h = 2;
            rc.u0 = r->u0 / 8 + 1;
            rc.v0 = r->v0 / 8 + 1;
            rc.u1 = r->u1 / 8 + 1;
            rc.v1 = r->v1 / 8 + 1;
            SprDrawRect3D(&rc, -1, -1, &v);
        }
    }
    SprFlush();
}
