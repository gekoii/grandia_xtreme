#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"



void BtlHudStatusIconDraw(void) {
    VuVec4 v;
    BtlHudStatusIcon *e;
    s32 i;
    f32 sc;
    f32 k;
    SprState *c;

    e = btlHudStatusIcons;
    SprBegin(0.0f, 0.0f);
    sprState.col[0].a = 0x40;
    for (i = 0; i < 0x10; i++, e++) {
        if (e->flags != 0) {
            BtlChrGetPartPos(&v, &btlUnits[i], 1);
            UtilWorldToScreen(&v, &v);
            c = &sprState;
            c->ox = v.x;
            c->oy = v.y - 64.0f;
            sc = 1.0f - e->timer * 0.00625f;
            if (e->index != 0xB) {
                k = sc * 5.3333335f;
            } else {
                k = sc * 10.666667f;
            }
            c->sy = k;
            c->sx = k;
            SprDrawRect(&menuStatusIcons[e->index], -12, -12);
        }
    }
    SprFlush();
}
