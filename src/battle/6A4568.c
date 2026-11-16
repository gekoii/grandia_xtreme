/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "sys.h"


void FxDefeatBurstDraw(void) {
    BtlUnit *u = btlUnits;
    FxDefeat *p = fxDefeat;
    FxDefeatPart *e;
    VuVec4 v;
    s32 i;
    s32 j;
    s32 t;
    s32 t2;
    f32 k;

    GphTexBind(0x34);
    SprBegin(0.0f, 0.0f);
    sprState.blend = 2;
    for (i = 0; i < 16; i++, u++, p++) {
        if (u->state <= 0) {
            continue;
        }
        if (p->parts == NULL) {
            continue;
        }
        e = p->parts;
        for (j = 0; j < 15; j++, e++) {
            if (e->life <= 0) {
                continue;
            }
            t = e->age;
            if (t < 5) {
                sprState.sx = t * 0.6f;
            } else if (t < 20) {
                sprState.sx = 3.0f - (t - 5) * (2.0f / 15.0f);
            } else {
                sprState.sx = 1.0f - 1.0f / (e->life - 20) * (t - 20);
            }
            sprState.sx *= e->sx;
            t = e->age;
            if (t < 5) {
                sprState.sy = t * 0.6f;
            } else if (t < 20) {
                sprState.sy = (t - 5) * 0.2f + 3.0f;
            } else {
                sprState.sy = 14.0f / (e->life - 20) * (t - 20) + 6.0f;
            }
            sprState.sy *= e->sy;
            t2 = e->age;
            if (t2 < 10) {
                k = t2 * 0.1f;
                sprState.col[0].b = 160;
                sprState.col[0].r = k * -10.0f + 70.0f;
                sprState.col[0].g = k * 80.0f + 0.0f;
            } else if (t2 < 15) {
                k = (t2 - 10) * 0.2f;
                sprState.col[0].b = 160;
                sprState.col[0].g = k * -80.0f + 80.0f;
                sprState.col[0].r = k * -60.0f + 60.0f;
            } else {
                k = 1.0f / (e->life - 15) * (t2 - 15);
                sprState.col[0].r = 0;
                sprState.col[0].g = 0;
                sprState.col[0].b = (k * -60.0f + 160.0f) * (1.0f - k);
            }
            sprState.flags = (sprState.flags & 0xFFFFFFFC) | (j & 3);
            v.x = e->pos[0];
            v.y = e->pos[1];
            v.z = e->pos[2];
            SprDrawRect3D(&ptclGlowRect, -1, -1, &v);
        }
    }
    SprFlush();
}
