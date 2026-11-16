/* cflags: -fno-strength-reduce */
/* -fno-strength-reduce: the unit loop counts up (with it on, the counter is reversed) */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudStatChangeDraw(void) {
    VuVec4 v;
    BtlHudStatChange *e;
    BtlUnit *u;
    s32 i;

    e = btlHudStatChanges;
    i = 0;
    SprBegin(0.0f, 0.0f);
    sprState.col[0].a = 0x80;
    u = btlUnits;
    for (; i < 0x10; i++, u++, e++) {
        if (e->bits != 0 && u->state >= 0x40 && e->index < 5) {
            BtlChrGetPartPos(&v, u, 1);
            UtilWorldToScreen(&v, &v);
            sprState.ox = v.x + 32.0f;
            sprState.oy = v.y + 16.0f;
            SprDrawRect(&btlHudStatChangeRects[e->index], -0x3E, -0x1E);
            if ((e->powUp >> e->index) & 1) {
                SprDrawRect(&btlHudStatChangeRects[6], -0x18, -8);
            } else if (e->powUp & (0x100 << e->index)) {
                SprDrawRect(&btlHudStatChangeRects[5], -8, -8);
            } else {
                SprDrawRect(&btlHudStatChangeRects[7], -0x14, -8);
            }
        }
    }
    SprFlush();
}
