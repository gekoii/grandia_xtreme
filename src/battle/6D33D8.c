#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudMarkerMissDraw(BtlHudMarker *fx) {
    VuVec4 v;
    f32 t;

    sprState.sy = 1.25f;
    sprState.sx = 1.25f;
    sprState.angle = 0;
    sprState.col[0].a = 0x80;
    if (fx->state > 0) {
        v.x = fx->x;
        v.y = 20.0f;
        v.z = fx->z;
        UtilWorldToScreen(&v, &v);
        sprState.ox = v.x;
        switch (fx->state) {
        case 1:
            t = fx->timer * 0.05f;
            sprState.col[0].a = t * 128.0f;
            t = 1.0f - t;
            t = t * t * 70.0f;
            sprState.oy = v.y - t;
            SprDrawRectList(btlHudMissLabel);
            break;
        case 2:
            if (fx->timer < 0x20) {
                sprState.col[0].a = 0x80;
            } else {
                sprState.col[0].a = (0x40 - fx->timer) * 128 * 0.03125f;
            }
            sprState.oy = v.y;
            SprDrawRectList(btlHudMissLabel);
            break;
        }
    }
}
