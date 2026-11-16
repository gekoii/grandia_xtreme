#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudMarkerEvadeDraw(BtlHudMarker *fx) {
    VuVec4 v;
    f32 t;

    sprState.sy = 1.25f;
    sprState.sx = 1.25f;
    sprState.col[0].a = 0x80;
    v.x = fx->x;
    v.y = 100.0f;
    v.z = fx->z;
    sprState.angle = 0;
    UtilWorldToScreen(&v, &v);
    sprState.oy = v.y;
    switch (fx->state) {
    case 0:
        t = fx->timer * 0.0625f;
        sprState.col[0].a = t * 64.0f;
        t = 1.0f - t;
        t = t * t * 200.0f;
        sprState.ox = v.x - t;
        SprDrawRectList(btlHudEvadeLabel);
        sprState.ox = v.x + t;
        SprDrawRectList(btlHudEvadeLabel);
        break;
    case 1:
        if (fx->timer < 0x20) {
            sprState.col[0].a = 0x80;
        } else {
            sprState.col[0].a = (0x40 - fx->timer) * 128 * 0.03125f;
        }
        sprState.ox = v.x;
        SprDrawRectList(btlHudEvadeLabel);
        break;
    }
}
