#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudMarkerLevelDownDraw(BtlHudMarker *fx) {
    VuVec4 v;

    sprState.sy = 1.0f;
    sprState.sx = 1.0f;
    sprState.col[0].a = 0x80;
    v.x = fx->x;
    v.y = 120.0f;
    v.z = fx->z;
    sprState.angle = 0;
    UtilWorldToScreen(&v, &v);
    sprState.ox = v.x;
    sprState.oy = v.y - fx->unk10;
    if (fx->state == 1 && fx->timer < 0x20) {
        sprState.col[0].a = fx->timer * 4;
    } else if (fx->state == 2 && fx->timer >= 0x31) {
        sprState.col[0].a = (0x50 - fx->timer) * 4;
    } else {
        sprState.col[0].a = 0x80;
    }
    SprDrawRect(&btlHudMarkerRects[4], -btlHudMarkerRects[4].w / 2, -btlHudMarkerRects[4].h / 2);
}
